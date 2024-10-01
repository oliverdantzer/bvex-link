from dotenv import load_dotenv
import sys
import os
from bord.src.run import run

def main():
    args = sys.argv[1:]
    if "--dev" in args:
        print("Running bord in development mode") 
        load_dotenv("./.env.development")
    
    load_dotenv("./.env")
    print(os.path.abspath(""))
    port = os.getenv("BORD_PORT")
    assert port is not None
    server_address = ('0.0.0.0', int(port))

    target_ip = os.environ.get("SOL_IP_ADDR")
    assert target_ip is not None
    target_port = os.getenv("SOL_PORT")
    assert target_port is not None
    target_address = (target_ip, int(target_port))
    run(server_address, target_address)
    

if __name__ == "__main__":
    main()