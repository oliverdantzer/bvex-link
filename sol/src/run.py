import asyncio
from signal import SIGINT, SIGTERM
from sol.src.server import Server
from sol.src.telemetry_buffer import TelemetryBuffer
from shared.src.encode_command import retransmit_segment_command, encode_commands
import sys
import signal
import threading
from gui import GUI


def run(server_address, target_address):
    server = Server(server_address, target_address)

    telemetry_buffer = TelemetryBuffer()

    loop = asyncio.get_event_loop()

    def signal_handler(*args):
        print("Keyboard Interrupt")
        loop.stop()

    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    async def main_loop(gui):
        while True:
            commands = []
            segments = server.receive_telemetry()
            for segment in segments:
                if gui.grid_size == (1, 1):
                    gui.set_total_num_segments(segment.data['sample_data_segment']['num_segments'])
                # print(segment.data['sample_data_segment']
                #       ['segment_data'].decode())
                telemetry_buffer.add_segment(segment)
                gui.add_seq_num(segment.data['sample_data_segment']['seq_num'])

            missing_seq_nums = telemetry_buffer.missing_seq_nums()
            if missing_seq_nums:
                for seq_num in missing_seq_nums:
                    if seq_num not in gui.missing_seq_nums:
                        gui.add_missing_seq_num(seq_num)
                assert telemetry_buffer.sample_id is not None
                commands.append(retransmit_segment_command(
                    telemetry_buffer.sample_id, list(missing_seq_nums)[:100]))

            if commands:
                server.send_packet(encode_commands(commands))
            if telemetry_buffer.is_complete():
                # Sort the items by the first value in each tuple
                sorted_segments = sorted(
                    telemetry_buffer.data_buffer.items(), key=lambda item: item[0])
                data = bytes()
                for key, data_segment in sorted_segments:
                    data += data_segment
                filename = "sample.txt"
                print(f"Finished downlink of sample, saving data to {filename}")
                with open(filename, 'w') as file:
                    file.write(data.decode())
                break
            # Add a small sleep to prevent a tight loop
            await asyncio.sleep(0.1)

    def run_main(gui):
        loop.run_until_complete(main_loop(gui))

    gui = GUI()
    threading.Thread(target=run_main, args=(gui,)).start()
    print("hello")
    gui.root.mainloop()
