# bvex link

## [onboard telemetry client documentation](https://oliverdantzer.github.io/bvex-link)

## Running in production

### Command and interface
To run the command module and an openmct interface, run the following, where onboard.server.ip is replaced with the actual IP address of the onboard server.
```bash
ONBOARD_SERVER_IP=onboard.server.ip docker compose -f command-compose.yml up
```