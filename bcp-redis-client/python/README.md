# Install dependencies

```bash
python3.13 -m venv venv
source venv/bin/activate
python -m pip install -r requirements.txt
```

# Tests
[Start redis server](../../redis-instructions.md)

```bash
python -m pytest test/test_sample.py
```

## manual tests

```bash
python manual_tests/test_sin_set.py
```

# Production
Redis daemon
