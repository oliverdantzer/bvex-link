import pytest
import redis
import os
import tempfile
from bcp_redis_client.sample import set_sample_file
from bvex_codec.sample import Sample, WhichDataType


# Test fixtures
@pytest.fixture
def redis_client():
    r = redis.Redis(host="localhost", port=6379, db=0)
    yield r
    r.flushdb()  # Clean up after tests


@pytest.fixture
def metric_id():
    return "test_metric"

def test_files(redis_client, metric_id):
    for filename in ["strawberries.png", "test.txt"]:
        file_path = os.path.join(os.path.dirname(__file__), "assets", filename)
        _test_file(redis_client, metric_id, file_path)

def _test_file(redis_client, metric_id, file_path):
    # Set the sample file
    set_sample_file(redis_client, metric_id, file_path)

    # Verify the sample was set in Redis
    sample_key = f"sample-cache:{metric_id}"
    assert redis_client.exists(sample_key)

    # Get the sample from Redis
    sample_data = redis_client.get(sample_key)
    assert sample_data is not None  # Should have data
    sample = Sample.model_validate_json(sample_data)
    
    # Verify sample metadata
    assert sample.metadata.metric_id == metric_id
    assert sample.metadata.which_data_type == WhichDataType.FILE
    
    # Verify file data
    assert sample.data.filename == os.path.basename(file_path)
    assert len(sample.data.data) == os.path.getsize(file_path)


def test_set_sample_file_empty(redis_client, metric_id):
    # Create an empty temporary file
    with tempfile.NamedTemporaryFile(delete=False) as f:
        empty_file = f.name

    try:
        set_sample_file(redis_client, metric_id, empty_file)
        
        # Verify the sample was set in Redis
        sample_key = f"sample-cache:{metric_id}"
        assert redis_client.exists(sample_key)
        
        # Get the sample from Redis
        sample_data = redis_client.get(sample_key)
        assert sample_data is not None
        sample = Sample.model_validate_json(sample_data)
        
        # Verify file data for empty file
        assert sample.data.filename == os.path.basename(empty_file)
        assert len(sample.data.data) == 0
    finally:
        # Clean up the temporary file
        os.unlink(empty_file)


def test_set_sample_file_nonexistent(redis_client, metric_id):
    # Try to set a non-existent file
    with pytest.raises(FileNotFoundError):
        set_sample_file(redis_client, metric_id, "nonexistent_file.txt") 