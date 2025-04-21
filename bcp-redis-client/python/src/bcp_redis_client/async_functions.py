import redis.asyncio as redis
from typing import Set
import asyncio


async def get_all_metric_ids(r: redis.Redis) -> Set[str]:
    metric_keys: Set[str] = set()
    cursor = 0
    while True:
        cursor, keys = await r.scan(match="sample-cache:*", cursor=cursor)
        # Decode bytes to strings
        metric_keys.update(key.decode("utf-8") for key in keys)
        if cursor == 0:
            break

    metric_ids: Set[str] = set()
    for key in metric_keys:
        parts = key.split(":")
        if len(parts) == 2 and parts[0] == "sample-cache":
            metric_ids.add(parts[1])

    return metric_ids
