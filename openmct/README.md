# BVEX OpenMCT Command Interface

## Production

The following steps should be followed to run this module

### 1. Build the Docker image:
```bash
docker build \
--build-arg TELEMETRY_SERVER_IP=your.server.ip \
--build-arg TELEMETRY_SERVER_PORT=8000 \
-t openmct .
```
> For running the interface on the **same host as the
> command server**, you can pass no command line arguments,
> which defaults the environment variables specifying the address
> of the telemetry server to localhost:8000
> ```bash
> docker build -t openmct .
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