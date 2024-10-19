#include "send_server.hpp"
#include <boost/make_shared.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

boost::shared_ptr<std::string> get_message()
{
    return boost::shared_ptr<std::string>(new std::string("Hello"));
}

send_server::send_server(boost::asio::io_service &io_service,
                         Telemetry &telemetry,
                         Command &command,
                         boost::asio::ip::port_type port,
                         boost::asio::ip::address& target_address,
                         boost::asio::ip::port_type target_port,
                         size_t bps)
    : socket_(io_service, udp::endpoint(udp::v4(), port)),
      telemetry_(telemetry),
      remote_endpoint_(target_address, target_port),
      timer_(io_service),
      command_(command)
{
    send_server::start_send();
}

void send_server::start_send()
{
    std::string message = telemetry_.pop();
    boost::shared_ptr<std::string> message_ptr = boost::make_shared<std::string>(message);
    // calls this.handle_send, giving 
    // it the shared_ptr to the message
    // once the data is handed off to the OS
    // networking stack for transmission
    socket_.async_send_to(boost::asio::buffer(*message_ptr), remote_endpoint_,
                          boost::bind(&send_server::handle_send,
                                      this, message_ptr,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

// Handles
void send_server::handle_send(boost::shared_ptr<std::string> /*message*/,
                              const boost::system::error_code &error,
                              std::size_t sent_size /*bytes_transferred*/)
{
    if (error)
    {
        std::cerr << "Error code" << error.to_string() << "on receive msg: " << error.message() << std::endl;
    }
    // Schedule sending of next packet, given the number of bytes sent
    send_server::schedule_send(sent_size);
}

// At the point this is called, we have already waited for async_send_to_ to give
// the packet to the OS. Given the number of bytes sent in the last packet
// and our desired bps, calculate the time until the next packet
// can be sent assuming the OS sends the previous packet at
// our desired bps. Then, schedule the next packet to be sent
// at that time. Of course, the OS may have delays in sending the packet
// for whatever reason, so we may need to add add additional delay to
// ensure we don't lose the packet, given a hard cap of bps.
void send_server::schedule_send(std::size_t sent_size)
{
    auto bits_sent = sent_size * 8;
    auto seconds_until_sent = bits_sent / command_.get_bps();
    auto interval = std::chrono::milliseconds(seconds_until_sent * 1000);
#ifdef DEBUG
    std::cout << "Sent " << sent_size << " bytes to port " << remote_endpoint_.port() << std::endl;
    std::cout << "Waiting " << std::to_string(interval.count()) << "ms to send next packet" << std::endl;
#endif
    // Set the timer's expiry time relative to now.
    timer_.expires_after(interval);
    timer_.async_wait(boost::bind(&send_server::start_send, this));
}