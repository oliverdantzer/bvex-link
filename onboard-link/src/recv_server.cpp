#include "recv_server.hpp"
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::udp;

RecvServer::RecvServer(boost::asio::io_service &io_service, int port, std::size_t buffer_size)
    // initialize members
    : socket_(io_service, udp::endpoint(udp::v4(), port)),
      buffer_size_(buffer_size),
      recv_buffer_(buffer_size) // initialize buffer with buffer_size bytes
{
    start_recv();
}

void RecvServer::start_recv()
{
    // data is written into recv_buffer_, data length and error
    // codes are passed to handle_receive

    // remote_endpoint_ recieves the endpoint of the sender
    // in async_receive_from
    socket_.async_receive_from(
        boost::asio::buffer(&RecvServer::recv_buffer_[0], RecvServer::buffer_size_), RecvServer::remote_endpoint_,
        boost::bind(&RecvServer::handle_recv, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void RecvServer::handle_recv(const boost::system::error_code &error,
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
    start_recv();
}