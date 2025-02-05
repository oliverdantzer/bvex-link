#include "request_server.hpp"
#include <codec/requests/request.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

using boost::asio::ip::udp;

RequestServer::RequestServer(udp::socket& listen_socket,
    std::function<std::optional<std::vector<uint8_t>>(std::string metric_id)>
        get_latest_sample_response)
    : socket_(listen_socket),
      requester_endpoint_(),
      get_latest_sample_response_(get_latest_sample_response)
{
    start_recv();
}

/**
 * async waits for incoming packets on socket_
 * When a packet is received, it is written to recv_buffer and
 * handle_recv is called
 */
void RequestServer::start_recv()
{
    // function to call handle_recv, passing it error code and
    // the number of bytes received
    auto on_recv = boost::bind(&RequestServer::handle_recv, this,
                               boost::asio::placeholders::error,
                               boost::asio::placeholders::bytes_transferred);

    socket_.async_receive_from(
        boost::asio::buffer(&recv_buffer_[0], REQUEST_PB_H_MAX_SIZE),
        requester_endpoint_, on_recv);
}

/**
 * passes bytes_received bytes of buffer to message_handler_
 */
void RequestServer::handle_recv(const boost::system::error_code& error,
                                std::size_t bytes_received)
{
    if(!error) {
        std::vector<uint8_t> request_enc(recv_buffer_.begin(),
                                         recv_buffer_.begin() + bytes_received);
        std::optional<Request> request = decode_request(request_enc);
        if(request) {
            handle_request(*request);
        }
    }
    // recurse
    start_recv();
}

void RequestServer::handle_request(Request& request)
{
    std::optional<std::vector<uint8_t>> response_enc =
        get_latest_sample_response_(request.metric_id);
    if(response_enc) {
            auto response_enc_ptr = std::make_shared<std::vector<uint8_t>>(*response_enc);
        auto handle_completion_callback =
            boost::bind(&RequestServer::handle_sent, this, response_enc_ptr,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred);
        socket_.async_send_to(boost::asio::buffer(*response_enc_ptr),
                              requester_endpoint_, handle_completion_callback);
    }
}

void RequestServer::handle_sent(
    std::shared_ptr<std::vector<uint8_t>> /*response_enc*/,
    const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
{
    if(error) {
        std::cerr << "Error sending response: " << error.message() << std::endl;
    }
}