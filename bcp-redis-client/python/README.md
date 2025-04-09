# Install dependencies

```bash
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

# Tests
Start redis (su required):
```bash
docker run -d --name redis -p 6379:6379 redis:7.4
docker exec -it redis redis-cli
```
```bash

pip install -e .
python -m pytest test/test_sample.py
```

# Development
Redis insight (su required):
```bash
docker run -d --name redisinsight -p 5540:5540 redis/redisinsight:latest
```