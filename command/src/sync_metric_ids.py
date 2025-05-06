import asyncio
from bvex_codec.telecommand import GetMetricIds, Telecommand
from bvex_codec.telemetry import Telemetry, WhichTMMessageType, MetricIds
from bvex_codec.sample import Sample, WhichDataType, PrimitiveData
from typing import Callable
from dataclasses import dataclass


@dataclass
class MetricInfo:
    metric_id: str
    bps: int = 0
    updated = asyncio.Event()  # indicates if this metric info has been updated

class MetricIdsStore:
    def __init__(self):
        self.metrics: dict[str, MetricInfo] = {}
        self.updated = asyncio.Event() # indicates if metric ids were updated

    # if new metric ids are different from current,
    # updates metric ids and sets updated flag
    def update(self, latest_metric_ids: list[str]):
        new_metric_ids = set(latest_metric_ids) - set(self.metrics.keys())
        if new_metric_ids:
            self.updated.set()
            for metric_id in new_metric_ids:
                self.metrics[metric_id] = MetricInfo(metric_id=metric_id)

    def update_bps(self, metric_id: str, bps: int):
        for metric_info in self.metrics.values():
            if metric_info.metric_id == metric_id:
                metric_info.bps = bps
                metric_info.updated.set()
                self.updated.set()
                break
        if self.updated.is_set():
            return True
        else:
            return False
    
    def get_metrics(self) -> set[MetricInfo]:
        return set(self.metrics.values())
    
    def get_updated_metrics (self) -> set[MetricInfo]:
        updated_metrics = set()
        if self.updated.is_set():
            for metric_info in self.metrics.values():
                if metric_info.updated.is_set():
                    updated_metrics.add(metric_info)
                    metric_info.updated.clear()
        return updated_metrics


async def sync_metric_ids(remote_addr: tuple[str, int], metric_ids: MetricIdsStore):
    reader, writer = await asyncio.open_connection(remote_addr[0], remote_addr[1])
    cmd = GetMetricIds()
    tc = Telecommand.from_command(cmd)
    writer.write(tc.model_dump_json().encode())
    await writer.drain()
    while True:
        try:
            async with asyncio.timeout(10.0):
                data = await reader.read(4096)
                if data:
                    telemetry = Telemetry.model_validate_json(data.decode())
                    if isinstance(telemetry.data, MetricIds):
                        metric_ids.update(telemetry.data.metric_ids)
        except asyncio.TimeoutError:
            pass
        await asyncio.sleep(10.0)
