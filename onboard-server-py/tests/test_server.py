import pytest
import asyncio
import bcp_redis_client
import redis
from bvex_codec.telecommand import Subscribe
from bvex_codec.telemetry import Telemetry
from bvex_codec.sample import Sample, WhichDataType, PrimitiveData
import subprocess

subprocess.run(["python3", "server.py"])


@pytest.mark.asyncio
async def test_subscribe_sample():
    # onboard ----
    r = redis.Redis()
    bcp_redis_client.set_sample_primitive(r, "test", 1)

    # ground ----
    reader, writer = await asyncio.open_connection("localhost", 8888)
    cmd = Subscribe(metric_id="test")
    writer.write(cmd.model_dump_json().encode())
    await writer.drain()

    data = await reader.read(4096)
    telemetry = Telemetry.model_validate_json(data.decode())
    assert telemetry.which_type == "sample"
    assert isinstance(telemetry.data, Sample)
    assert telemetry.data.metadata.metric_id == "test"
    assert telemetry.data.metadata.which_data_type == WhichDataType.PRIMITIVE
    assert isinstance(telemetry.data.data, PrimitiveData)
    assert telemetry.data.data.value == 1
