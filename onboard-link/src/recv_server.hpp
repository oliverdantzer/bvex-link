#ifndef RECV_SERVER_H
#define RECV_SERVER_H

#include <boost/asio.hpp>
#include <vector>

#include "command.hpp"

using boost::asio::ip::udp;

class RecvServer
{
  public:
    RecvServer(boost::asio::io_service& io_service, int port,
               std::size_t buffer_size = 4096);

  private:
    void start_recv();
    void handle_recv(const boost::system::error_code& error,
                     std::size_t bytes_recvd);

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::size_t buffer_size_;
    std::vector<char> recv_buffer_;
};

#endif // RECV_SERVER_H