import asyncio
import os
from .cli import CLI
from .subscriber import subscribe_all
from .sync_metric_ids import MetricIdsStore, sync_metric_ids
from bvex_codec.sample import Sample
from dotenv import load_dotenv
from .telemetry_server import TelemetryServer
from .config import config


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

    async def handle_sample(sample: Sample):
        # await sample_store.store_sample(sample)
        await telemetry_server.add_sample(sample)
        # print(sample)
    subscribe_all_task = asyncio.create_task(
        subscribe_all(remote_addr, metric_ids_store, handle_sample))

    await asyncio.gather(sync_metric_ids_task, subscribe_all_task, 
                         telemetry_server.run())
