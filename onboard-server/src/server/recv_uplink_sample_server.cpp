#include "recv_uplink_sample_server.hpp"
#include "../decode_onboard_telemetry/decode_sample_pb.hpp"
#include "recv_server.hpp"
#include <iostream>
#include <memory>

RecvUplinkSampleServer::RecvUplinkSampleServer(
    boost::asio::io_service& io_service, Command& command, int port,
    std::size_t buffer_size)
    : command_(command),
      recv_server_(io_service,
                   std::bind(&RecvUplinkSampleServer::handle_message, this,
                             std::placeholders::_1, std::placeholders::_2),
                   port, buffer_size) {};

void RecvUplinkSampleServer::handle_message(
    std::unique_ptr<std::vector<uint8_t>> message, size_t size)
{
    std::unique_ptr<SampleData> sample = decode_payload(std::move(message), size);
    command_.add_sample(std::move(sample));
}
