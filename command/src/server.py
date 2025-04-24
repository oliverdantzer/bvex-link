import asyncio
import os
from cli import CLI
from subscriber import subscribe_all
from sync_metric_ids import MetricIdsStore, sync_metric_ids
from bvex_codec.sample import Sample
from store_sample import SampleStore


async def run_server():
    metric_ids = set()
    remote_addr = ("localhost", 8888)
    metric_ids_store = MetricIdsStore()
    sample_store = SampleStore(os.path.relpath("data"))
    sync_metric_ids_task = asyncio.create_task(
        sync_metric_ids(remote_addr, metric_ids_store))

    def store_sample(sample: Sample):
        sample_store.store_sample(sample)
        print(sample)
    subscribe_all_task = asyncio.create_task(
        subscribe_all(remote_addr, metric_ids_store, store_sample))
    
    await asyncio.gather(sync_metric_ids_task, subscribe_all_task)


if __name__ == "__main__":
    asyncio.run(run_server())
