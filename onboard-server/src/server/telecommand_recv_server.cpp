#include "telecommand_recv_server.hpp"
#include "recv_server.hpp"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TelecommandRecvServer::TelecommandRecvServer(
    boost::asio::io_service& io_service, boost::asio::ip::port_type port,
    Command& command, std::size_t buffer_size)
    : recv_server_(io_service, port,
                   std::bind(&TelecommandRecvServer::handle_message, this,
                             std::placeholders::_1),
                   buffer_size),
      command_(command) {};

void TelecommandRecvServer::handle_message(
    std::unique_ptr<std::vector<uint8_t>> message)
{
    std::string message_str(message->begin(), message->end());
    json telecommand = json::parse(message_str);

    if(telecommand.contains("set_bps")) {
        command_.set_bps(telecommand["set_bps"]["value"]);
    } else if(telecommand.contains("set_max_pkt_size")) {
        command_.set_max_packet_size(telecommand["set_max_pkt_size"]["value"]);
    } else {
        std::cerr << "Telecommand not recognized: " << message_str << std::endl;
    }
}
