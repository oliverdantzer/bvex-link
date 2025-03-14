import os
from sample_data import SampleMetadata

SAMPLE_STORE_DIR = os.path.abspath("data")


class SampleStore:
    def __init__(self) -> None:
        if not os.path.exists(SAMPLE_STORE_DIR):
            os.makedirs(SAMPLE_STORE_DIR)


def store_sample(data: bytes, sample: SampleMetadata):
    metric_dir = os.path.join(SAMPLE_STORE_DIR, f"metric-{sample.metric_id}")
    if not os.path.exists(metric_dir):
        os.makedirs(metric_dir)
    sample_file = os.path.join(metric_dir, f"sample-{sample.sample_id}.json")
    with open(sample_file, "wb") as f:
        f.write(data)


def store_file(metric_dir: str, data: bytes, sample: SampleMetadata):
    sample_file = os.path.join(
        metric_dir, f"sample-{sample.sample_id}.{sample.data_type}")
    with open(sample_file, "wb") as f:
        f.write(data)


def store_primitive(data: bytes, sample: SampleMetadata):
    metric_dir = os.path.join(SAMPLE_STORE_DIR, f"metric-{sample.metric_id}")
    if not os.path.exists(metric_dir):
        os.makedirs(metric_dir)
    sample_file = os.path.join(
        metric_dir, f"sample-{sample.sample_id}.{sample.data_type}")
    with open(sample_file, "wb") as f:
        f.write(data)
