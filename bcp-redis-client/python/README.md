# Install dependencies

```bash
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

# Tests
Start redis server on port 6379.
- Create and start: `sudo docker run -d --name redis -p 6379:6379 redis:7.4`
- Start: `sudo docker start redis`

```bash
pip install -e .
python -m pytest test/test_sample.py
```

# Development
Redis insight:
```bash
sudo docker run -d --name redisinsight -p 5540:5540 redis/redisinsight:latest
xdg-open http://localhost:5540
```
Redis CLI:
```
sudo docker exec -it redis redis-cli
```