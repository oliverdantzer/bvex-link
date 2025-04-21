import asyncio
from bcp_redis_client import SampleSubscriber, get_all_metric_ids
import socket
from typing import Callable
import redis
from bvex_codec.sample import Sample
from bvex_codec.telecommand import Telecommand, Subscribe, GetMetricIds, Cancel, End
from bvex_codec.telemetry import Telemetry
from pydantic import ValidationError
import asyncio
import queue

port = 8888
r = redis.Redis()


async def downlink_latest_sample(
    writer: asyncio.StreamWriter, get_sample: Callable[[], Sample | None]
) -> int:
    sample = get_sample()
    if sample is None:
        return 0
    telemetry = Telemetry(which_type="sample", data=sample)
    data = telemetry.model_dump_json().encode()
    writer.write(data)
    await writer.drain()
    return len(data)


# downlinks samples at approximately get_bps() bitrate
async def downlink_loop(
    writer: asyncio.StreamWriter,
    subscriber: SampleSubscriber,
    get_bps: Callable[[], int],
):
    # get socket we are writing to
    sock = writer.get_extra_info("socket")

    # downlink forever
    while True:
        bytes_sent = await downlink_latest_sample(writer, subscriber.get_sample)

        if sock is not None:
            # set socket send buffer to twice the size of the sample
            # to ensure the socket cannot queue many samples in the send buffer
            # (older samples are not useful anymore)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, bytes_sent * 2)
            print("setsockopt applied to client socket")

        if bytes_sent == 0:
            # sleep for 1 second if no data was sent
            # to avoid busy-waiting on a new sample
            await asyncio.sleep(1)
        else:
            # sleep for the time it approximately takes to transmit the sample
            await asyncio.sleep(bytes_sent / get_bps())


async def handle_subscribe(writer: asyncio.StreamWriter, cmd: Subscribe
):
    with SampleSubscriber(r, cmd.metric_id) as subscriber:
        return downlink_loop(writer, subscriber, lambda:10000)

async def handle_get_metric_ids(writer: asyncio.StreamWriter):
    metric_ids = list(get_all_metric_ids(r))
    telemetry = Telemetry(which_type="metric_ids", data=metric_ids)
    data = telemetry.model_dump_json().encode()
    writer.write(data)
    await writer.drain()

async def run_command(writer: asyncio.StreamWriter, cmd: Telecommand) -> None:
    if isinstance(cmd, Subscribe):
        await handle_subscribe(writer, cmd)
    elif isinstance(cmd, GetMetricIds):
        await handle_get_metric_ids(writer)


async def handle_connection(reader: asyncio.StreamReader, writer: asyncio.StreamWriter):
    async def read() -> Telecommand | None:
        data = await reader.read(4096)
        message = data.decode()
        try:
            tc = Telecommand.model_validate_json(message)
            return tc
        except ValidationError as e:
            print(e)
            return None

    current_task = None
    command_queue = queue.Queue()
    while True:
        tc = await read()
        if tc is not None:
            if isinstance(tc, Cancel) or isinstance(tc, End):
                if current_task is not None:
                    current_task.cancel()
                    current_task = None
                    command_queue.queue.clear()
                
                if isinstance(tc, End):
                    break
            else:
                if current_task is None:
                    command = command_queue.get()
                    current_task = asyncio.create_task(run_command(writer, command))
                else:
                    command_queue.put(tc)
    writer.close()
    await writer.wait_closed()


async def main():
    server = await asyncio.start_server(handle_connection, "127.0.0.1", port)
    # handle_connection is called for each new connection established

    addrs = ", ".join(str(sock.getsockname()) for sock in server.sockets)
    print(f"Serving on {addrs}")

    async with server:
        await server.serve_forever()


asyncio.run(main())
