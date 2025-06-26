# BCP Redis Client

## Installing and using this package

To install this package into your currently active python environment, run the following in this directory:
```bash
pip install -e .
```

This package provides the following imports:
```python
from bcp_redis_client.asyncio import get_all_metric_ids, get_sample
from bcp_redis_client.sample import set_sample_file, set_sample_file_from_bytes, get_sample, SampleSubscriber, set_sample_primitive
```

Example usage can be found in the tests directory.

## Developing this package

### Install dependencies

```bash
python3.13 -m venv venv
source venv/bin/activate
python -m pip install -r requirements.txt
```

### Tests
[Start redis server](../../redis-instructions.md)

```bash
python -m pytest
```

### manual tests

```bash
python manual_tests/test_sin_set.py
```
```bash
python manual_tests/test_images.py
```