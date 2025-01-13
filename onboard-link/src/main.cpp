#include "command.hpp"
#include "server/recv_uplink_sample_server.hpp"
#include "server/send_server.hpp"
#include "telemetry.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    try {
        if(argc != 5) {
            std::cerr
                << "Usage: " << argv[0]
                << " <recv_port> <send_port> <target_address> <target_port>\n";
            return 1;
        }
        const int recv_port = std::atoi(argv[1]);
        const int send_port = std::atoi(argv[2]);
        std::string target_address_str = argv[3];
        boost::asio::ip::address target_address;
        if(target_address_str == "localhost") {
            // not IPv6 compatible, shouldn't be an issue
            target_address = boost::asio::ip::make_address("127.0.0.1");
        } else {
            target_address = boost::asio::ip::make_address(argv[3]);
        }
        const int target_port = std::atoi(argv[4]);

        Command command = Command(100000, 100);
        Telemetry telemetry = Telemetry(command);
        boost::asio::io_service io_service;
        SendServer send_server(io_service, telemetry, command, send_port,
                               target_address, target_port);
        RecvUplinkSampleServer recv_server(io_service, command, recv_port);
        io_service.run();
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}