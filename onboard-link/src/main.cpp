#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "send_server.hpp"
#include "recv_server.hpp"

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
        
        boost::asio::io_service io_service;
        send_server send_server(io_service, send_port, target_port, 8);
        recv_server recv_server(io_service, recv_port);
        io_service.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}