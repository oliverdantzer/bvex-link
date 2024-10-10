import socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.sendto("hello", ('localhost', 12345))