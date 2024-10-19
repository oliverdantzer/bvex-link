import sys
import os
from run import run

def main():
    if len(sys.argv) != 4:
        print(f"Usage: python {
              sys.argv[0]} <target_ip> <target_port> <source_port>")
        sys.exit(1)

    args = sys.argv[1:]

    target_ip = args[0]
    target_port = args[1]
    target_address = (target_ip, int(target_port))

    source_port = args[2]
    server_address = ('0.0.0.0', int(source_port))

    run(server_address, target_address)


if __name__ == "__main__":
    main()
