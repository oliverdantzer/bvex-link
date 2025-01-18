#include "onboard_telemetry_recv_server.hpp"
#include "../decode_onboard_telemetry/decode_sample_pb.hpp"
#include "recv_server.hpp"
#include <iostream>
#include <memory>

OnboardTelemetryRecvServer::OnboardTelemetryRecvServer(
    boost::asio::io_service& io_service, boost::asio::ip::port_type port,
    Command& command, std::size_t buffer_size)
    : recv_server_(io_service, port,
                   std::bind(&OnboardTelemetryRecvServer::handle_message, this,
                             std::placeholders::_1),
                   buffer_size),
      command_(command) {};

void OnboardTelemetryRecvServer::handle_message(
    std::unique_ptr<std::vector<uint8_t>> message)
{
    std::unique_ptr<SampleData> sample = decode_payload(std::move(message));
    command_.add_sample(std::move(sample));
}
