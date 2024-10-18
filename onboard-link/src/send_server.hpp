#ifndef UDP_SEND_SERVER_H
#define UDP_SEND_SERVER_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

boost::shared_ptr<std::string> get_message();

class send_server
{
public:
    send_server(boost::asio::io_service &io_service,
                boost::asio::ip::port_type port,
                boost::asio::ip::port_type target_port,
                std::size_t bps);

private:
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::vector<char> recv_buffer_;
    boost::asio::steady_timer timer_;
    size_t bps_;

    void start_send();
    void handle_send(boost::shared_ptr<std::string> message,
                     const boost::system::error_code &error,
                     std::size_t sent_size);
    void schedule_send(std::size_t sent_size);
    void set_bps(std::size_t bps);
};

#endif // UDP_SEND_SERVER_H