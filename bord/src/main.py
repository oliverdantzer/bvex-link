from dotenv import load_dotenv
import sys
import asyncio
import os
from bord.src.server import Server
from bord.src.telemetry import telemetry_loop
from bord.src.telemetry_buffer import SendBuffer

async def main():
    args = sys.argv[1:]
    if "--dev" in args:
        print("Running bord in development mode") 
        load_dotenv("./.env.development")
    load_dotenv()
    
    port = os.getenv("BORD_PORT")
    assert port is not None
    server_address = ('0.0.0.0', int(port))

    target_ip = os.environ.get("SOL_IP_ADDR")
    assert target_ip is not None
    target_port = os.getenv("SOL_PORT")
    assert target_port is not None
    target_address = (target_ip, int(target_port))

    server = Server(server_address, target_address)
    send_buffer = SendBuffer()
    
    # Run both loops concurrently
    await asyncio.gather(
        telemetry_loop(send_buffer),
        server.downlink_loop(send_buffer),
        server.receive_loop(send_buffer)
    )

if __name__ == "__main__":
    print('fdsf')
    asyncio.run(main())