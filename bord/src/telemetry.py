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
        self.lastDownlinkTime = -math.inf


metrics: set[TelemetryMetric] = {
    TelemetryMetric("msg", reading.get_msg_reading),
}


def add_telemetry(send_buffer: SendBuffer):
    t = time.time()
    for metric in metrics:
        timeSinceLastDownlink = t - metric.lastDownlinkTime
        if timeSinceLastDownlink >= (1/metric.sample_rate):
            send_buffer.add(metric.getReading())
            metric.lastDownlinkTime = t
