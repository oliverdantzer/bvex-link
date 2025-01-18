#include "command.hpp"
#include "server/onboard_telemetry_recv_server.hpp"
#include "server/send_server.hpp"
#include "telemetry.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
    try {
        if(argc != 5) {
            std::cerr
                << "Usage: " << argv[0]
                << " <recv_port> <send_port> <target_address> <target_port>\n";
            return 1;
        }
        const boost::asio::ip::port_type recv_port =
            static_cast<uint_least16_t>(std::atoi(argv[1]));
        const boost::asio::ip::port_type send_port =
            static_cast<uint_least16_t>(std::atoi(argv[2]));
        std::string target_address_str = argv[3];
        boost::asio::ip::address target_address;
        if(target_address_str == "localhost") {
            // not IPv6 compatible, shouldn't be an issue
            target_address = boost::asio::ip::make_address("127.0.0.1");
        } else {
            target_address = boost::asio::ip::make_address(argv[3]);
        }
        const boost::asio::ip::port_type target_port =
            static_cast<uint_least16_t>(std::atoi(argv[4]));

        Command command = Command(100000, 100);
        Telemetry telemetry = Telemetry(command);
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), send_port));
        udp::endpoint target_endpoint(target_address, target_port);
        SendServer send_server(io_service, socket, target_endpoint, telemetry,
                               command);
        OnboardTelemetryRecvServer recv_server(io_service, recv_port, command);
        io_service.run();
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}