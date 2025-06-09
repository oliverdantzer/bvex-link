# bvex link

## [onboard telemetry client documentation](https://oliverdantzer.github.io/bvex-link)

## Running in production

### Running onboard server as a daemon

> This will be run on the ophiuchius machine

First build the onboard server service
```bash
docker compose -f onboard-server-compose.yml up --build
```
Create systemd service file for the onboard server
```bash
cp ./onboard-server.service /etc/systemd/system/onboard-server.service
```
Reload systemd and Enable the Service
```bash
sudo systemctl daemon-reload
sudo systemctl start onboard-server.service
sudo systemctl enable onboard-server.service
```
Verify the Service
```bash
sudo systemctl status onboard-server.service
```

### Command and interface

> This will be run on the saggitarius machine

To run the command module and an openmct interface, run the following, where onboard.server.ip is replaced with the actual IP address of the onboard server.
```bash
ONBOARD_SERVER_IP=onboard.server.ip docker compose -f command-compose.yml up
```
Then navigate to the link (http://localhost/8080) to view the openmct, and interact with the terminal to send commands. You can type `help` in the terminal to view the list of commands.

### Sending and requesting data from the onboard server

> This will be used on the ophiuchius and saggitarius machines

To interact with the onboard server from python code, view the instructions [./bcp-redis-client/python/](here).

Using C code, you must install the library [./bcp-redis-client/c/](here).