import asyncio
from bvex_codec.telecommand import GetMetricIds, Telecommand
from bvex_codec.telemetry import Telemetry, WhichTMMessageType, MetricIds
from bvex_codec.sample import Sample, WhichDataType, PrimitiveData
from typing import Callable
from dataclasses import dataclass, field
from .subscriber import subscribe


@dataclass
class MetricInfo:
    metric_id: str
    bps: int = 0

    def __hash__(self) -> int:
        return hash(self.metric_id)


class Metric:
    def __init__(self, metric_id: str):
        self.metric_info = MetricInfo(metric_id=metric_id)

    async def subscribe(self, remote_addr, handle_sample):
        await subscribe(remote_addr,
                        self.metric_info.metric_id,
                        self.metric_info.bps,
                        handle_sample=handle_sample)


class MetricSubscribeHandler:
    def __init__(self, remote_addr: tuple[str, int], handle_sample: Callable, bps_distribution_mode: str, max_bps: int):
        self.max_bps = max_bps
        self.remote_addr = remote_addr
        self.handle_sample = handle_sample
        self.metrics: dict[str, MetricInfo] = {}
        self.metric_subscriptions: dict[str, asyncio.Task] = {}
        self.updated = asyncio.Event()  # indicates if metric ids were updated
        self.bps_distribution_mode: str = bps_distribution_mode

    async def add_metric(self, metric_id: str):
        metric_info = MetricInfo(metric_id=metric_id)
        self.metrics[metric_id] = metric_info
        self.metric_subscriptions[metric_id] = asyncio.create_task(subscribe(self.remote_addr,
                                                                             metric_id,
                                                                             metric_info.bps,
                                                                             handle_sample=self.handle_sample))
        if self.bps_distribution_mode == "fair":
            await self.redistribute_bps_fairly()

    # if new metric ids are different from current,
    # updates metric ids and sets updated flag

    async def update_metric_id_list(self, latest_metric_ids: list[str]):
        new_metric_ids = set(latest_metric_ids) - set(self.metrics.keys())
        if new_metric_ids:
            self.updated.set()
            for metric_id in new_metric_ids:
                await self.add_metric(metric_id)
    
    async def update_bps(self, metric_id: str, bps: int):
        if metric_id in self.metrics:
            self.metrics[metric_id].bps = bps
            old_task = self.metric_subscriptions[metric_id]
            old_task.cancel()
            try:
                await old_task
            except asyncio.CancelledError:
                pass
            self.metric_subscriptions[metric_id] = asyncio.create_task(
                subscribe(self.remote_addr, metric_id, bps,
                          handle_sample=self.handle_sample)
            )

    async def set_max_bps(self, bps: int):
        if bps != self.max_bps:
            self.max_bps = bps
            await self.redistribute_bps_fairly()
        

    async def redistribute_bps_fairly(self):
        metric_ids = self.metrics.keys()
        bps_per_metric = self.max_bps // len(metric_ids)
        async with asyncio.TaskGroup() as tg:
            for metric_id in self.metrics.keys():
                tg.create_task(self.update_bps(metric_id, bps_per_metric))

    def get_metrics(self) -> set[MetricInfo]:
        return set(self.metrics.values())

    async def sync_metric_ids(self):
        reader, writer = await asyncio.open_connection(self.remote_addr[0], self.remote_addr[1])
        cmd = GetMetricIds()
        tc = Telecommand.from_command(cmd)
        writer.write(tc.model_dump_json().encode())
        await writer.drain()
        while True:
            try:
                async with asyncio.timeout(10.0):
                    data = await reader.read(4096)
                    if data:
                        telemetry = Telemetry.model_validate_json(
                            data.decode())
                        if isinstance(telemetry.data, MetricIds):
                            await self.update_metric_id_list(telemetry.data.metric_ids)
            except asyncio.TimeoutError:
                pass
            await asyncio.sleep(10.0)

    async def __aenter__(self):
        self.sync_task = asyncio.create_task(self.sync_metric_ids())

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        self.sync_task.cancel()
        try:
            await self.sync_task
        except asyncio.CancelledError:
            pass
        for task in self.metric_subscriptions.values():
            task.cancel()
            try:
                await task
            except asyncio.CancelledError:
                pass
