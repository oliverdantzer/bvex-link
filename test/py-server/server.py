import socket
import select
import threading
from generated.sample_pb2 import Sample

# Define server address and port
server_address = ('localhost', 3000)

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the server address
sock.bind(server_address)

print(f"UDP server listening on {server_address}")

def message_handler(data, addr):
    sample = Sample()
    print(f"Received {len(data)} bytes from {addr}")
    print("data: ", repr(data))
    # sample.ParseFromString(data)
    # print("Parsed data: ", sample)

MAX_MESS_LEN = 1024
stop_flag = threading.Event()

def server_loop():
    while not stop_flag.is_set():
        try:
            data, addr = sock.recvfrom(MAX_MESS_LEN)
            threading.Thread(target=message_handler, args=(data, addr)).start()
        except socket.error:
            break

server_thread = threading.Thread(target=server_loop)
server_thread.start()

try:
    while True:
        pass
except KeyboardInterrupt:
    print("Shutting down server...")
    stop_flag.set()
    sock.shutdown(socket.SHUT_WR)
    sock.close()
    server_thread.join()
    print("Server shut down.")