import redis
from bcp_redis_client.encode import (
    Primitive,
    encode_sample,
    decode_sample,
    PrimitiveType,
    Sample,
)
import time
from threading import Thread, Lock
from typing import Generic, TypeVar


def set_sample_primitive(r: redis.Redis, metric_id: str, value: Primitive):
    """
    Sets the sample primitive in the Redis cache.
    """
    sample = encode_sample(metric_id, time.time(), value)
    r.set(
        f"sample-cache:{metric_id}",
        sample,
    )
    r.publish(f"sample-channel:{metric_id}", sample)


class SampleSubscriber(Generic[PrimitiveType]):
    """
    Subscribes to the sample channel and keeps track of the latest sample.
    """

    latest_sample: Sample[PrimitiveType] | None

    def __init__(self, r: redis.Redis, metric_id: str):
        self.primitive_type = PrimitiveType
        self.r = r
        self.metric_id = metric_id
        self.p = r.pubsub()
        self.p.subscribe(f"sample-channel:{metric_id}")
        self.latest_sample_lock = Lock()
        self.latest_sample = None

    def __enter__(self):
        self.thread = Thread(target=self._run)
        self.thread.start()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.p.unsubscribe()
        self.thread.join()

    def _run(self):
        for message in self.p.listen():
            if message["type"] == "message":
                sample = decode_sample(message["data"])
                assert sample.metric_id == self.metric_id
                if not isinstance(sample.value, self.primitive_type.__constraints__):
                    raise TypeError(
                        f"Received value type {type(sample.value)} does not match expected type {self.primitive_type.__constraints__}"
                    )
                with self.latest_sample_lock:
                    self.latest_sample = sample

    def get_sample(self) -> Sample[PrimitiveType]:
        with self.latest_sample_lock:
            if self.latest_sample is None:
                raise ValueError("No sample available")
            return self.latest_sample
