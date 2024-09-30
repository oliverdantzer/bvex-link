import socket
import os
import logging
import bord.src.telemetry_buffer

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class Server:
    def __init__(self, server_address: tuple[str, int], target_address: tuple[str, int]):
        self.server_address = server_address
        self.target_address = target_address
        
        # Create a UDP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
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
        try:
            self.sock.sendto(data, self.target_address)
        except Exception as e:
            logger.error(f"Failed to send packet: {e}")

    def pop_recieve_buffer(self):
        data, address = self.sock.recvfrom(4096)

        if address != self.recieved_ip:
            self.recieved_ip = address
            logger.info(f"Receiving data from {address}")
        
        print(data)

    async def receive_loop(self, send_buffer):
        logger.info("Processing UDP packets")

        try:
            while True:
                self.pop_recieve_buffer()
                send_buffer.retransmit(self.retransmit_seq_nums_by_sample_id)
        except Exception as e:
            logger.error(f"Error receiving data: {e}")
        finally:
            logger.info("Receive stopped")
    
    async def downlink_loop(self, send_buffer):
        logger.info("Downlinking telemetry data")
        try:
            while True:
                send_buffer.pop_payload()
                self.send_packet(send_buffer.pop_payload())
        except Exception as e:
            logger.error(f"Error sending data: {e}")
        finally:
            logger.info("Downlink stopped")
