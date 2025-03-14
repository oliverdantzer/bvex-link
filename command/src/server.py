import asyncio
import json
import socket
from dataclasses import dataclass
from typing import Any, Callable
import threading
import logging
import queue
from cli import CLI
from sample_data import Ack, Sample, SampleMetadata, Segment, SegmentMetadata
from sample_receiver import SampleReceiver

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)


class RecvServer(asyncio.DatagramProtocol):
    remote_addr: Any = None
    handle_sample: Callable[[Sample], None]
    set_remote_addr: Callable[[Any], None]

    def __init__(
        self,
        handle_sample: Callable[[Sample], None],
        set_remote_addr: Callable[[Any], None],
    ):
        super().__init__()
        self.handle_sample = handle_sample
        self.set_remote_addr = set_remote_addr

    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        if self.remote_addr != addr:
            if self.remote_addr is None:
                self.remote_addr = addr
                self.set_remote_addr(addr)
            else:
                logger.warning(
                    "Datagram received from multiple hosts, discarding")
                return
        sample_json = json.loads(data.decode())
        logger.debug(f"Received packet from {addr}, size: {len(data)} bytes")
        logger.debug(f"Packet metadata - metric_id: {sample_json['metric_id']}, sample_id: {sample_json['sample_id']}, "
                     f"segment: {sample_json['segment']['seqnum']}/{sample_json['segment']['num_segments']}")
        segment_metadata = SegmentMetadata(
            sample_json["segment"]["seqnum"], sample_json["segment"]["num_segments"]
        )
        sample_metadata = SampleMetadata(
            sample_json["metric_id"],
            sample_json["sample_id"],
            sample_json["timestamp"],
            sample_json["data_type"],
        )
        segment = Segment(segment_metadata, bytes(
            sample_json["segment"]["data"]))
        sample = Sample(sample_metadata, segment)
        self.handle_sample(sample)


@dataclass
class SetBps:
    bps: int


@dataclass
class SetMaxPacketSize:
    max_pkt_size: int


Telecommand = SetBps | SetMaxPacketSize


class SendServer:
    remote_addr: tuple[str, int] | None = None
    get_ack: Callable[[], Ack]
    sock: socket.socket
    bits_per_second: int = 100
    remote_addr_set: asyncio.Event = asyncio.Event()
    command_queue: queue.Queue[Telecommand] = queue.Queue()

    def __init__(self, get_ack: Callable[[], Ack]):
        self.get_ack = get_ack
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def set_remote_addr(self, addr: tuple[str, int]):
        # TODO: Change send and receive addr on server
        # to be same so I dont have to do this workaround
        self.remote_addr = ('127.0.0.1', 3001)
        self.remote_addr_set.set()

    def set_bps(self, bps: int):
        self.command_queue.put(SetBps(bps))

    def set_max_pkt_size(self, pkt_size: int):
        self.command_queue.put(SetMaxPacketSize(pkt_size))

    async def send_loop(self) -> None:
        logger.info("Starting send loop")
        await self.remote_addr_set.wait()
        logger.info(f"Remote addr received: {self.remote_addr}")
        loop = asyncio.get_running_loop()
        while True:
            obj: Any = {}
            if not self.command_queue.empty():
                command = self.command_queue.get()
                if isinstance(command, SetBps):
                    obj["set_bps"] = {}
                    obj["set_bps"]["bps"] = command.bps
                elif isinstance(command, SetMaxPacketSize):
                    obj["set_max_pkt_size"] = {}
                    obj["set_max_pkt_size"]["max_pkt_size"] = command.max_pkt_size
                else:
                    raise ValueError("Invalid command")
                json_string = json.dumps(obj)
            else:
                ack = self.get_ack()
                obj["ack"] = {}
                obj["ack"]["metric_id"] = ack.metric_id
                obj["ack"]["sample_id"] = ack.sample_id
                obj["ack"]["seqnums"] = ack.seqnums
            json_string = json.dumps(obj)
            # print("sending packet: ", json_string)
            data = json_string.encode()
            assert data
            if self.remote_addr:
                # self.sock.sendto(data, self.remote_addr)
                await loop.sock_sendto(self.sock, data, self.remote_addr)
            time_to_send = len(data) / self.bits_per_second
            # Adjust the sleep time as needed
            await asyncio.sleep(time_to_send)


async def run_server():
    receiver = SampleReceiver()

    send_server = SendServer(receiver.get_ack)

    # Bind to localhost on UDP port 3000
    local_addr = ("127.0.0.1", 3004)
    loop = asyncio.get_running_loop()
    transport, _ = await loop.create_datagram_endpoint(
        lambda: RecvServer(receiver.handle_sample,
                           send_server.set_remote_addr),
        local_addr,
    )
    logger.info(f"Server listening on {local_addr}")

    # Schedule the send_loop to run concurrently
    send_task = asyncio.create_task(send_server.send_loop())

    cli = CLI(send_server.set_bps, send_server.set_max_pkt_size)
    cli_thread = threading.Thread(target=cli.run)
    cli_thread.start()

    try:
        await asyncio.sleep(3600)  # Run for 1 hour
    finally:
        transport.close()
        send_task.cancel()
        await send_task


if __name__ == "__main__":
    asyncio.run(run_server())
