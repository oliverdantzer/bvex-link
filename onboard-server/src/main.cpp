#include "command.hpp"
#include "server/onboard_telemetry_recv_server.hpp"
#include "server/send_server.hpp"
#include "server/telecommand_recv_server.hpp"
#include "server/request_server.hpp"
#include "telemetry.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
    try {
        if(argc != 3) {
            std::cerr << "Usage: " << argv[0]
                      << "<target_address> <target_port>\n";
            return 1;
        }
        const boost::asio::ip::port_type onboard_telemetry_recv_port = 3000;
        const boost::asio::ip::port_type telecommand_recv_port = 3001;
        const boost::asio::ip::port_type send_port = 3002;
        std::string target_address_str = argv[1];
        boost::asio::ip::address target_address;
        if(target_address_str == "localhost") {
            // not IPv6 compatible, shouldn't be an issue
            target_address = boost::asio::ip::make_address("127.0.0.1");
        } else {
            target_address = boost::asio::ip::make_address(argv[1]);
        }
        const boost::asio::ip::port_type target_port =
            static_cast<uint_least16_t>(std::atoi(argv[2]));
        if(target_port == onboard_telemetry_recv_port ||
           target_port == telecommand_recv_port || target_port == send_port) {
            std::cerr << "Target port cannot be the same as any of the server "
                         "ports\n";
            return 1;
        }
        Command command = Command(100000, 100);
        Telemetry telemetry = Telemetry(command);
        boost::asio::io_service io_service;
        
        OnboardTelemetryRecvServer onboard_telemetry_recv_server(
            io_service, onboard_telemetry_recv_port, command);

        TelecommandRecvServer telecommand_recv_server(
            io_service, telecommand_recv_port, command);

        RequestServer request_server(
            io_service, telecommand_recv_port, 
            std::bind(&Command::get_latest_sample_response, &command, std::placeholders::_1));

        udp::socket socket(io_service, udp::endpoint(udp::v4(), send_port));
        udp::endpoint target_endpoint(target_address, target_port);
        SendServer send_server(io_service, socket, target_endpoint, telemetry,
                               command);
        io_service.run();
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}