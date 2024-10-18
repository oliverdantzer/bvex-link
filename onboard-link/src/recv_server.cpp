#include "recv_server.hpp"

using boost::asio::ip::udp;

recv_server::recv_server(boost::asio::io_service &io_service, int port, std::size_t buffer_size)
    // initialize members
    : socket_(io_service, udp::endpoint(udp::v4(), port)),
      buffer_size_(buffer_size),
      recv_buffer_(buffer_size) // initialize buffer with buffer_size bytes
{
    start_receive();
}

void recv_server::start_receive()
{
    // data is written into recv_buffer_, data length and error
    // codes are passed to handle_receive

    // remote_endpoint_ recieves the endpoint of the sender
    // in async_receive_from
    socket_.async_receive_from(
        boost::asio::buffer(&recv_server::recv_buffer_[0], recv_server::buffer_size_), recv_server::remote_endpoint_,
        boost::bind(&recv_server::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void recv_server::handle_receive(const boost::system::error_code &error,
                                 std::size_t bytes_recvd /*bytes_transferred*/)
{
    // We will get error boost::asio::error::message_size
    // if the message is too big to fit in the buffer
    if (!error)
    {
        // char *received_data(recv_buffer_.data());
        // If we don't do a shared ptr, message might be destroyed
        // before the async send is done
        boost::shared_ptr<std::string> message(
            new std::string(recv_buffer_.data(), bytes_recvd));
        std::cout << "Received message: " << *message << std::endl;
    }
    else
    {
        std::cerr << "Error code" << error.to_string() << "on receive msg: " << error.message() << std::endl;
    }

    // recurse
    start_receive();
}