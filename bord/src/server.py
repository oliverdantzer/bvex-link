import socket
import logging
from shared.src.decode_command import decode_commands
from shared.src.encode_command import Command
import random

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class Server:
    def __init__(self, server_address: tuple[str, int], target_address: tuple[str, int]):
        self.server_address = server_address
        self.target_address = target_address
        
        # Create a UDP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setblocking(False)  # Don't wait until recieve data
        self.sock.bind(self.server_address)
        logger.info(f"Server initialized")
        logger.info(
            f"UDP socket opened at udp://{self.server_address[0]}:{self.server_address[1]}")
        logger.info(
            f"Target: udp://{self.target_address[0]}:{self.target_address[1]}")
        self.recieved_ip = None
        
        self.retransmit_seq_nums_by_sample_id: set[str] = set()
    
    def __del__(self):
        self.sock.close()
        logger.info("Socket closed")

    def send_packet(self, data: bytes):
        """Send a packet to sol."""
        print("Sending packet ", data.decode("utf-8"))
        if (random.random() < 0.1):  # Simulate packet loss 90%
            self.sock.sendto(data, self.target_address)
        else:
            print("Packet lost")

    def pop_recieve_buffer(self) -> bytes | None:
        try:
            data, address = self.sock.recvfrom(4096)

            if address != self.recieved_ip:
                self.recieved_ip = address
                logger.info(f"Receiving data from {address}")
            if len(data) == 0:
                return None
            return data
        except BlockingIOError:
            return None
        except ConnectionResetError as e:
            print(e)
            return None

    def receive_commands(self) -> list[Command]:
        commands_data = self.pop_recieve_buffer()
        if not commands_data:
            return []
        commands = decode_commands(commands_data)
        return commands
