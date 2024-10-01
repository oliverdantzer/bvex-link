import asyncio
from signal import SIGINT, SIGTERM
from sol.src.server import Server
from sol.src.telemetry_buffer import TelemetryBuffer
from shared.src.encode_command import retransmit_segment_command, encode_commands
import sys
import signal


def run(server_address, target_address):
    server = Server(server_address, target_address)

    telemetry_buffer = TelemetryBuffer()

    loop = asyncio.get_event_loop()

    def signal_handler(*args):
        print("Keyboard Interrupt")
        loop.stop()

    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    async def main_loop():
        while True:
            commands = []
            segments = server.receive_telemetry()
            for segment in segments:
                print(segment.data['sample_data_segment']
                      ['segment_data'].decode())
                telemetry_buffer.add_segment(segment)
            
            missing_seq_nums = telemetry_buffer.missing_seq_nums()
            if missing_seq_nums:
                assert telemetry_buffer.sample_id is not None
                commands.append(retransmit_segment_command(
                    telemetry_buffer.sample_id, list(missing_seq_nums)))
            
            
            if commands:
                server.send_packet(encode_commands(commands))
            if telemetry_buffer.is_complete():
                # Sort the items by the first value in each tuple
                sorted_segments = sorted(
                    telemetry_buffer.data_buffer.items(), key=lambda item: item[0])
                data = bytes()
                for key, data_segment in sorted_segments:
                    data += data_segment
                print(data.decode())
            # Add a small sleep to prevent a tight loop
            await asyncio.sleep(0.1)

    try:
        loop.run_until_complete(main_loop())
    except KeyboardInterrupt:
        print("Terminating the loop on Ctrl+C")
    finally:
        loop.close()
