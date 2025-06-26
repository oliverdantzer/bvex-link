from fastapi import HTTPException
from bvex_codec import Sample
import redis.asyncio as redis
import dateutil.parser as dp
from bvex_codec.sample import FileSample
from .config import config

# TODO: give telemetry history manager aenter and aexit
# clear history on aenter
# refactor store_file_sample to into this manager

TELEMETRY_HISTORY_KEY = "telemetry_history"



class TelemetryHistoryManager:
    def __init__(self):
        self.r = redis.Redis()
    
    async def clear_history(self):
        # Clear telemetry history keys on initialization
        keys_to_clear = await self.r.keys(f"{TELEMETRY_HISTORY_KEY}:*")
        if keys_to_clear:
            await self.r.delete(*keys_to_clear)

    async def add_sample(self, sample: Sample):
        print("new sample :)")
        await self.r.zadd(f"{TELEMETRY_HISTORY_KEY}:{sample.metadata.metric_id}",
                                       {sample.model_dump_json(): sample.metadata.timestamp})

    # expects start and end time in ISO-8601 format
    async def http_endpoint(self, metric_id: str, start: str, end: str):
        try:
            start_timestamp = dp.parse(start).timestamp()
            end_timestamp = dp.parse(end).timestamp()
        except ValueError:
            raise HTTPException(
                400, detail="Invalid date format. Expected ISO-8601 string.")
        sample_jsons: list[str] = await self.r.zrangebyscore(f"{TELEMETRY_HISTORY_KEY}:{metric_id}", start_timestamp, end_timestamp)
        samples = [Sample.model_validate_json(sample) for sample in sample_jsons]
        return samples
