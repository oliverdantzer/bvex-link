import asyncio
import os
from .cli import CLI
from .sync_metric_ids import MetricSubscribeHandler
from bvex_codec.sample import Sample
from .telemetry_server import TelemetryServer
from .config import config
from .cli import CLI
import threading
import queue


async def run_server():
    remote_addr: tuple[str, int] = (
        config.ONBOARD_SERVER_IP,
        config.ONBOARD_SERVER_PORT,
    )
    telemetry_server = TelemetryServer(
        config.TELEMETRY_SERVER_PORT)
    bps_distribution_mode = config.DEFAULT_BPS_DISTRIBUTION_MODE
    max_bps = config.DEFAULT_MAX_BPS

    async def handle_sample(sample: Sample):
        # await sample_store.store_sample(sample)
        await telemetry_server.add_sample(sample)

    metric_ids_store = MetricSubscribeHandler(
        remote_addr,
        handle_sample,
        bps_distribution_mode,
        max_bps)
    metric_id_bps = {}

    update_bps_queue: asyncio.Queue[tuple[str, int]] = asyncio.Queue()
    set_max_bps_queue: asyncio.Queue[int] = asyncio.Queue()

    def update_bps_sync(metric_id: str, bps: int):
        update_bps_queue.put_nowait((metric_id, bps))
    def set_max_bps_sync(bps: int):
        set_max_bps_queue.put_nowait(bps)

    async def handle_update_bps_queue():
        while True:
            update = await update_bps_queue.get()
            await metric_ids_store.update_bps(
                update[0], update[1])
    async def handle_set_max_bps_queue():
        while True:
            bps = await set_max_bps_queue.get()
            await metric_ids_store.set_max_bps(bps)
        
    cli = CLI(
        handle_set_bps=update_bps_sync,
        handle_set_max_bps=set_max_bps_sync,
        get_metric_infos=metric_ids_store.get_metrics,
    )
    cli_thread = threading.Thread(
        target=cli.run,
    )
    cli_thread.start()
    async with metric_ids_store:
        await asyncio.gather(
            handle_update_bps_queue(),
            handle_set_max_bps_queue(),
            telemetry_server.run())
    cli_thread.join()
