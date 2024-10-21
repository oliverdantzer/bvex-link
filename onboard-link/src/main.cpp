#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "send_server.hpp"
#include "recv_server.hpp"
#include "telemetry.hpp"
#include "command.hpp"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: " << argv[0] << " <recv_port> <send_port> <target_port>\n";
            return 1;
        }
        const int recv_port = std::atoi(argv[1]);
        const int send_port = std::atoi(argv[2]);
        const int target_port = std::atoi(argv[3]);
        Command command = Command(100);
        Telemetry telemetry = Telemetry(command);
        boost::asio::io_service io_service;
        SendServer send_server(io_service, telemetry, command, send_port, target_port, 400);
        RecvServer recv_server(io_service, recv_port);
        io_service.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}