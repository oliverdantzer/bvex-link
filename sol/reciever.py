import socket
import os

port: str = os.environ.get("PORT") or ""

if port is None:
    raise ValueError("PORT environment variable is not set")

def udp_server(host='0.0.0.0'):
    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket to the address and port
    server_address = (host, int(port))
    sock.bind(server_address)

    print(f"Listening for UDP packets on {host}:{port}")

    while True:
        # Receive data
        data, address = sock.recvfrom(4096)

        print(f"Received {len(data)} bytes from {address}")
        data.decode('utf-8')
