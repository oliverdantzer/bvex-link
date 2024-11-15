#pragma once

#include "command.hpp"
#include "recv_server.hpp"
#include <boost/asio.hpp>

class RecvUplinkSampleServer
{
  public:
    RecvUplinkSampleServer(boost::asio::io_service& io_service,
                           Command& command, int port,
                           std::size_t buffer_size = 4096);

  private:
    void handle_message(boost::shared_ptr<std::vector<uint8_t>> message, size_t size);
    RecvServer recv_server_;
    Command command_;
};