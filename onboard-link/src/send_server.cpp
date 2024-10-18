#include "send_server.hpp"

boost::shared_ptr<std::string> get_message()
{
    return boost::shared_ptr<std::string>(new std::string("Hello"));
}

send_server::send_server(boost::asio::io_service &io_service,
                         boost::asio::ip::port_type port,
                         boost::asio::ip::port_type target_port,
                         size_t bps)
    : socket_(io_service, udp::endpoint(udp::v4(), port)),
      remote_endpoint_(udp::v4(), target_port),
      timer_(io_service),
      bps_(bps)
{
    send_server::start_send();
}

void send_server::start_send()
{
    boost::shared_ptr<std::string> message = get_message();
    socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                          boost::bind(&send_server::handle_send,
                                      this, message,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

void send_server::handle_send(boost::shared_ptr<std::string> /*message*/,
                              const boost::system::error_code &error,
                              std::size_t sent_size /*bytes_transferred*/)
{
    if (error)
    {
        std::cerr << "Error code" << error.to_string() << "on receive msg: " << error.message() << std::endl;
    }
    send_server::schedule_send(sent_size);
}

void send_server::schedule_send(std::size_t sent_size)
{
    auto bits_sent = sent_size * 8;
    auto seconds_until_sent = bits_sent / bps_;
    auto interval = std::chrono::milliseconds(seconds_until_sent * 1000);
#ifdef DEBUG
    std::cout << "Sent " << sent_size << " bytes to port " << remote_endpoint_.port() << std::endl;
    std::cout << "Waiting " << std::to_string(interval.count()) << "ms to send next packet" << std::endl;
#endif
    // Set the timer's expiry time relative to now.
    timer_.expires_after(interval);
    timer_.async_wait(boost::bind(&send_server::start_send, this));
}