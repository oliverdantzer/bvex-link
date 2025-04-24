import asyncio
import threading
from cli import CLI
from subscriber import subscribe_all
from sync_metric_ids import MetricIdsStore, sync_metric_ids
from bvex_codec.sample import Sample

def store_sample(sample: Sample):
    print(sample)

async def run_server():
    metric_ids = set()
    remote_addr = ("localhost", 8888)
    metric_ids_store = MetricIdsStore()
    sync_metric_ids_task = asyncio.create_task(sync_metric_ids(remote_addr, metric_ids_store))
    subscribe_all_task = asyncio.create_task(subscribe_all(remote_addr, metric_ids_store, store_sample))
    await asyncio.gather(sync_metric_ids_task, subscribe_all_task)


if __name__ == "__main__":
    asyncio.run(run_server())
