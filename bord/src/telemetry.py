from typing import TypeVar, Generic, Callable
import bord.src.reading as reading
from bord.src.telemetry_buffer import SendBuffer
from shared.src.sample import Sample
import math
import time

T = TypeVar('T')


class TelemetryMetric(Generic[T]):
    def __init__(self, metric_id: str, getReading: Callable[[], Sample]):
        self.metric_id = metric_id
        self.getReading = getReading
        self.sample_rate = 1/10  # Hz
        self.timeSinceLastDownlink = math.inf


metrics: set[TelemetryMetric] = {
    TelemetryMetric("img", reading.get_img_reading),
}


async def telemetry_loop(send_buffer: SendBuffer):
    dt = 0
    t = time.time()
    while True:
        dt = time.time() - t
        t = time.time()
        for metric in metrics:
            metric.timeSinceLastDownlink += dt
            if metric.timeSinceLastDownlink >= (1/metric.sample_rate):
                send_buffer.add(metric.getReading())
                metric.timeSinceLastDownlink = 0
