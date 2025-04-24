```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

# tests
first start the server
```
python3 run.py
```
and [start redis](../bcp-redis-client/python/README.md)

then run the tests
```
pytest
```