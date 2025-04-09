import pytest
import redis
import time
from typing import TypeVar, Generic
from bcp_redis_client.sample import set_sample_primitive, SampleSubscriber
from bcp_redis_client.encode import Primitive, Sample, SampleType


# Test fixtures
@pytest.fixture
def redis_client():
    r = redis.Redis(host="localhost", port=6379, db=0)
    yield r
    r.flushdb()  # Clean up after tests


@pytest.fixture
def metric_id():
    return "test_metric"


# Tests for set_sample_primitive
def test_set_sample_primitive_int(redis_client, metric_id):
    value = 42
    set_sample_primitive(redis_client, metric_id, value)

    # Verify the sample was set in Redis
    sample_key = f"sample-cache:{metric_id}"
    assert redis_client.exists(sample_key)

    # Get the sample from Redis
    sample_data = redis_client.get(sample_key)
    assert sample_data is not None  # Should have data


def test_set_sample_primitive_float(redis_client, metric_id):
    value = 3.14
    set_sample_primitive(redis_client, metric_id, value)
    sample_key = f"sample-cache:{metric_id}"
    assert redis_client.exists(sample_key)


def test_set_sample_primitive_str(redis_client, metric_id):
    value = "test string"
    set_sample_primitive(redis_client, metric_id, value)
    sample_key = f"sample-cache:{metric_id}"
    assert redis_client.exists(sample_key)


def test_set_sample_primitive_bool(redis_client, metric_id):
    value = True
    set_sample_primitive(redis_client, metric_id, value)
    sample_key = f"sample-cache:{metric_id}"
    assert redis_client.exists(sample_key)


# Tests for SampleSubscriber
def test_sample_subscriber_int(redis_client, metric_id):
    # Create subscriber
    with SampleSubscriber[int](redis_client, metric_id) as subscriber:
        # Set a sample
        value = 42
        set_sample_primitive(redis_client, metric_id, value)

        # Give some time for the subscriber to receive the message
        time.sleep(0.1)

        # Get the sample
        sample = subscriber.get_sample()
        assert isinstance(sample, Sample)
        assert sample.metric_id == metric_id
        assert sample.type == SampleType.primitive
        assert sample.value == value
        assert isinstance(sample.value, int)


def test_sample_subscriber_float(redis_client, metric_id):
    with SampleSubscriber[float](redis_client, metric_id) as subscriber:
        value = 3.14
        set_sample_primitive(redis_client, metric_id, value)
        time.sleep(0.1)

        sample = subscriber.get_sample()
        assert sample.value == value
        assert isinstance(sample.value, float)


def test_sample_subscriber_str(redis_client, metric_id):
    with SampleSubscriber[str](redis_client, metric_id) as subscriber:
        value = "test string"
        set_sample_primitive(redis_client, metric_id, value)
        time.sleep(0.1)

        sample = subscriber.get_sample()
        assert sample.value == value
        assert isinstance(sample.value, str)


def test_sample_subscriber_bool(redis_client, metric_id):
    with SampleSubscriber[bool](redis_client, metric_id) as subscriber:
        value = True
        set_sample_primitive(redis_client, metric_id, value)
        time.sleep(0.1)

        sample = subscriber.get_sample()
        assert sample.value == value
        assert isinstance(sample.value, bool)


def test_sample_subscriber_type_error(redis_client, metric_id):
    with SampleSubscriber[int](redis_client, metric_id) as subscriber:
        # Set a string value when expecting an int
        value = "not an int"
        set_sample_primitive(redis_client, metric_id, value)
        time.sleep(0.1)

        # Should raise TypeError when getting the sample
        with pytest.raises(TypeError):
            subscriber.get_sample()


def test_sample_subscriber_no_sample_error(redis_client, metric_id):
    with SampleSubscriber[int](redis_client, metric_id) as subscriber:
        # Try to get a sample before any have been set
        with pytest.raises(ValueError, match="No sample available"):
            subscriber.get_sample()
