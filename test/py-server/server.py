import socket
from generated.sample_pb2 import Sample

# Define server address and port
server_address = ('localhost', 3000)

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the server address
sock.bind(server_address)

print(f"UDP server listening on {server_address}")

while True:
    # Wait for a connection
    data, address = sock.recvfrom(4096)

    sample = Sample()
    
    print(f"Received {len(data)} bytes from {address}")
    print("data: ", repr(data))
    sample.ParseFromString(data)
    print("data: ", sample)

    # # Optionally, send a response back to the client
    # if data:
    #     sent = sock.sendto(data, address)
    #     print(f"Sent {sent} bytes back to {address}")