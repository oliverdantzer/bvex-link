#ifndef RECV_SERVER_H
#define RECV_SERVER_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <vector>

using boost::asio::ip::udp;

class recv_server
{
public:
    recv_server(boost::asio::io_service &io_service, int port, std::size_t buffer_size = 4096);

private:
    void start_receive();
    void handle_receive(const boost::system::error_code &error, std::size_t bytes_recvd);

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::size_t buffer_size_;
    std::vector<char> recv_buffer_;
};

#endif // RECV_SERVER_H