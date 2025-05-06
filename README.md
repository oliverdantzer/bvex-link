# bvex link

## [onboard telemetry client documentation](https://oliverdantzer.github.io/bvex-link)

## Running in production

### Command and interface
To run the command module and an openmct interface, run the following, where onboard.server.ip is replaced with the actual IP address of the onboard server.
```bash
ONBOARD_SERVER_IP=onboard.server.ip docker compose -f command-compose.yml up
```
Then navigate to the link (http://localhost/8080) to view the openmct, and interact with the terminal to send commands.

### Running onboard server as a daemon
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