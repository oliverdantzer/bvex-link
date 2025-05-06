# BVEX OpenMCT Command Interface

## Environment variables
To run in development or production, the following environment variables must be defined:
- `TELEMETRY_SERVER_IP`
- `TELEMETRY_SERVER_PORT`

This should usually be done by putting a file named `.env` in this directory. The code will automatically read environment variables from this file.

An example of the contents of `.env` can be found in the file `.env.example`.

> Environment variables defined before running the code will be overriden by those defined in the `.env` file.

## Production

The following steps should be followed to run this module

### 1. Build the Docker image:
```bash
docker build -t openmct .
```
> To build without a .env file in this directory, you can pass environment
> variables through the build command:
> ```bash
> docker build \
> -e TELEMETRY_SERVER_IP=telemetry.server.ip \
>    TELEMETRY_SERVER_PORT=8000 \
> -t openmct .
> ```
### 2. Run the container:
```bash
docker run -p 8080:8080 openmct
```

## Development

### 1. Install node js
- `curl -fsSL https://fnm.vercel.app/install | bash` [install fnm](https://github.com/Schniz/fnm#installation)
- `fnm install 22` install node v22
- `fnm use 22`

### 2. Run in development mode
```
npm i
npm run dev
```