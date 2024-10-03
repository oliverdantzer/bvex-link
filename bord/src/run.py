import asyncio
import concurrent.futures
import threading
import time
from signal import SIGINT, SIGTERM
from bord.src.server import Server
from bord.src.telemetry import add_telemetry
from bord.src.telemetry_buffer import SendBuffer, PopEmptySendBufferError
import sys, signal

def run(server_address, target_address):
    server = Server(server_address, target_address)
    send_buffer = SendBuffer(payload_size=1024)
    send_buffer.set_max_segment_data_size(50)
    add_telemetry(send_buffer)
    ack_sample = False
    
    loop = asyncio.get_event_loop()
    
    def signal_handler(*args):
        print("Keyboard Interrupt")
        loop.stop()

    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)
    
    async def main_loop():
        while True:
            payload = None
            try:
                payload = send_buffer.get_payload()
            except PopEmptySendBufferError:
                print("Empty buffer")
                if not ack_sample:
                    send_buffer.sample_buffers[0].seq_num = 0
            if payload and payload != b'[]':
                server.send_packet(payload)
            commands = server.receive_commands()
            if commands:
                for command in commands:
                    print(f"Received command: {command}")
                    if (command['command'] == "retransmit_segment"):
                        if len(command['seq_nums']) == 0:
                            return
                        send_buffer.set_retransmit_seq_nums(command['sample_id'], command['seq_nums'])
                    if (command['command'] == "ack_sample"):
                        ack_sample = True
                        
            await asyncio.sleep(0.1)  # Add a small sleep to "prevent a tight loop"?

    try:
        loop.run_until_complete(main_loop())
    except KeyboardInterrupt:
        print("Terminating the loop on Ctrl+C")
    finally:
        loop.close()