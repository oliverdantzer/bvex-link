# BVEX Onboard Server

Running the onboard server depends on a redis server running on the same host on port 6379. For running a development redis server, see [redis instructions](../bcp-redis-client/python/README.md).

## Running in production

### 1. Build the Docker image:
**FROM THE [ROOT REPOSITORY FOLDER](../)**, run the following:
```bash
docker build -t onboard-server -f onboard-server-py/Dockerfile .
```
### 2. Run the container:
```bash
docker run -p 8000:8000 onboard-server
```

## Running in development

### Setup

[Install Python version 3.13](../python-instructions.md)

Create virtual env, install requirements
`python3.13 -m venv venv
source venv/bin/activate
python -m pip install -r requirements.txt
`

### Running the onboard server
```
python run.py
```

### Running tests
```
pytest
```