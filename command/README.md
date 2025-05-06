# BVEX Command module

## Environment variables
To run in development or production, the following environment variables must be defined:
- `ONBOARD_SERVER_IP`
- `ONBOARD_SERVER_PORT`

This should be done by putting a file named `.env` in this directory. The code will automatically read environment variables from this file.

An example of the contents of `.env` can be found in the file `.env.example`.

> Environment variables defined before running `run.py` will be overriden by those defined in the `.env` file.

## Running in production

### 1. Build the Docker image:
**FROM THE [ROOT REPOSITORY FOLDER](../)**, run the following:
```bash
docker build -t command -f command/Dockerfile .
```
> To build without a .env file in this directory, you can pass environment
> variables through the build command:
> ```bash
> docker build \
> -e ONBOARD_SERVER_IP=onboard.server.ip \
>    ONBOARD_SERVER_PORT=3000 \
> -t command -f command/Dockerfile .
> ```
### 2. Run the container:
```bash
docker run -p 8000:8000 command
```

## Running in development
The following instructions are only tested on Debian-based linux machines.

[Install Python version 3.13](../python-instructions.md)

Create virtual env, install requirements
`python3.13 -m venv venv
source venv/bin/activate
python -m pip install -r requirements.txt
`

[Start redis server](../redis-instructions.md)

Running
```bash
python run.py
```