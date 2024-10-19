from server import Server


def run(server_address, target_address):
    server = Server(server_address, target_address)

    def main_loop():
        while True:
            server.receive_telemetry()

    main_loop()