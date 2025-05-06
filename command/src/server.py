import asyncio
import os
from .cli import CLI
from .subscriber import subscribe_all
from .sync_metric_ids import MetricIdsStore, sync_metric_ids
from bvex_codec.sample import Sample
from dotenv import load_dotenv
from .telemetry_server import TelemetryServer
from .config import config
from .cli import CLI
import threading


async def run_server():
    remote_addr: tuple[str, int] = (
        config.ONBOARD_SERVER_IP,
        config.ONBOARD_SERVER_PORT,
    )
    metric_ids_store = MetricIdsStore()
    sync_metric_ids_task = asyncio.create_task(
        sync_metric_ids(remote_addr, metric_ids_store))
    telemetry_server = TelemetryServer(
        config.TELEMETRY_SERVER_PORT)
    
    max_bps = 10000
    metric_id_bps = {}
    def set_max_bps(bps: int) -> str:
        nonlocal max_bps, metric_ids_store
        max_bps = bps
        msg = ""
        acc = 0
        for metric_info in metric_ids_store.get_metrics():
            acc += metric_info.bps
            if acc > max_bps:
                metric_ids_store.update_bps(
                    metric_info.metric_id, 0)
                msg += f"Metric id {metric_info.metric_id} is set to 0 bps\n"
        return msg
    cli = CLI(
        handle_set_bps=metric_ids_store.update_bps,
        handle_set_max_bps=set_max_bps,
        get_metric_info=metric_ids_store.get_metrics,
        )
    cli_thread = threading.Thread(
        target=cli.run,
    )
    cli_thread.start()

    async def handle_sample(sample: Sample):
        # await sample_store.store_sample(sample)
        await telemetry_server.add_sample(sample)
        # print(sample)
    subscribe_all_task = asyncio.create_task(
        subscribe_all(remote_addr, metric_ids_store, handle_sample))

    await asyncio.gather(sync_metric_ids_task, subscribe_all_task, 
                         telemetry_server.run())
    cli_thread.join()
