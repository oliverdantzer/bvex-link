import asyncio
import json
import socket
from typing import Any, Callable

from sample_data import Ack, Sample, SampleMetadata, Segment, SegmentMetadata
from sample_receiver import SampleReceiver


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
                print("Warning: datagram recieved from multiple hosts, discarding")
                return
        sample_json = json.loads(data.decode())
        segment_metadata = SegmentMetadata(
            sample_json["segment"]["seqnum"], sample_json["segment"]["num_segments"]
        )
        sample_metadata = SampleMetadata(
            sample_json["metric_id"],
            sample_json["sample_id"],
            sample_json["timestamp"],
            sample_json["data_type"],
        )
        segment = Segment(segment_metadata, bytes(sample_json["segment"]["data"]))
        sample = Sample(sample_metadata, segment)
        self.handle_sample(sample)


class SendServer:
    remote_addr: Any = None
    get_ack: Callable[[], Ack]
    sock: socket.socket
    bits_per_second: int = 100
    remote_addr_set: asyncio.Event = asyncio.Event()

    def __init__(self, get_ack: Callable[[], Ack]):
        self.get_ack = get_ack
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def set_remote_addr(self, addr):
        self.remote_addr = addr
        self.remote_addr_set.set()

    async def send_loop(self):
        await self.remote_addr_set.wait()
        while True:
            ack = self.get_ack()
            ack_object = {}
            ack_object["metric_id"] = ack.metric_id
            ack_object["sample_id"] = ack.sample_id
            ack_object["seqnums"] = ack.seqnums
            json_string = json.dumps(ack_object)
            print(json_string)
            data = json_string.encode()
            if self.remote_addr:
                self.sock.sendto(data, self.remote_addr)
            time_to_send = len(data) / self.bits_per_second
            # Adjust the sleep time as needed
            await asyncio.sleep(time_to_send)


async def run_server():
    print("Starting UDP server")

    receiver = SampleReceiver()

    send_server = SendServer(receiver.get_ack)

    # Bind to localhost on UDP port 3000
    loop = asyncio.get_running_loop()
    transport, _ = await loop.create_datagram_endpoint(
        lambda: RecvServer(receiver.handle_sample, send_server.set_remote_addr),
        local_addr=("127.0.0.1", 3000),
    )

    # Schedule the send_loop to run concurrently
    send_task = asyncio.create_task(send_server.send_loop())

    try:
        await asyncio.sleep(3600)  # Run for 1 hour
    finally:
        transport.close()
        send_task.cancel()
        await send_task


if __name__ == "__main__":
    asyncio.run(run_server())
