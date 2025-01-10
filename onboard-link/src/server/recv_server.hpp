#pragma once

#include <boost/asio.hpp>
#include <cstdint>
#include <vector>

#include "command.hpp"

using boost::asio::ip::udp;

class RecvServer
{
  public:
    RecvServer(boost::asio::io_service& io_service,
               std::function<void(boost::shared_ptr<std::vector<uint8_t>>, size_t)>
                   message_handler,
               int port, std::size_t buffer_size = 4096);

  private:
    void start_recv();
    void handle_recv(const boost::system::error_code& error,
                     std::size_t bytes_recvd);

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::size_t buffer_size_;
    std::vector<uint8_t> recv_buffer_;
    std::function<void(boost::shared_ptr<std::vector<uint8_t>>, size_t)>
        message_handler_;
};
