#include "recv_uplink_sample_server.hpp"
#include "decode_payload/decode_sample_pb.hpp"
#include "recv_server.hpp"

RecvUplinkSampleServer::RecvUplinkSampleServer(
    boost::asio::io_service& io_service, Command& command, int port,
    std::size_t buffer_size)
    : command_(command),
        recv_server_(io_service,
                     std::bind(&RecvUplinkSampleServer::handle_message, this,
                               std::placeholders::_1, std::placeholders::_2),
                     port, buffer_size)
    {

      };

void RecvUplinkSampleServer::handle_message(
    boost::shared_ptr<std::vector<uint8_t>> message, size_t size)
{
    boost::shared_ptr<SampleData> sample = decode_payload(message, size);
    command_.add_sample(sample);
}
