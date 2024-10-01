from dotenv import load_dotenv
import os
import sys

if __name__ == "__main__":
    args = sys.argv[1:]
    if "--dev" in args:
        # variables defined in first call of load_dotenv
        # will override definitions of same variables in
        # subsequent calls of load_dotenv
        print("Running sol in development mode")
        load_dotenv("./.env.development")
    load_dotenv()

    import reciever
    reciever.udp_server()
