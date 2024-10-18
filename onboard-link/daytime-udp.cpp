#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using boost::asio::ip::udp;

boost::shared_ptr<std::string> get_message()
{
  return boost::shared_ptr<std::string>(new std::string("Hello"));
}

class recv_server
{
public:
  // constructor
  recv_server(boost::asio::io_service &io_service, int port, std::size_t buffer_size = 4096)
      // initialize members
      : socket_(io_service, udp::endpoint(udp::v4(), port)),
        buffer_size_(buffer_size),
        recv_buffer_(buffer_size) // initialize buffer with buffer_size bytes
  {
    start_receive();
    const int bps = 10;
    std::time_t t_current = std::time(nullptr);
    std::time_t dt = 0;
    while (true)
    {
      dt = std::time(nullptr) - t_current;
      t_current = std::time(nullptr);

      boost::shared_ptr<std::string> message = get_message();
      if (can_send && dt * bps >= message.get()->size())
      {
        start_send(message);
      }
    }
    // get dt
    // if dt * bytes/s > prev send packet size, send new packet
  }

private:
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  std::size_t buffer_size_;
  bool can_send = true;
  size_t packet_size = 0;

  // Each char corresponds to a byte
  std::vector<char> recv_buffer_;

  void start_receive()
  {
    // data is written into recv_buffer_, data length and error
    // codes are passed to handle_receive

    // remote_endpoint_ recieves the endpoint of the sender
    // in async_receive_from
    socket_.async_receive_from(
        boost::asio::buffer(&recv_buffer_[0], buffer_size_), remote_endpoint_,
        boost::bind(&recv_server::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  void handle_receive(const boost::system::error_code &error,
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
  void start_send(boost::shared_ptr<std::string> message)
  {
    can_send = false;
    socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                          boost::bind(&recv_server::handle_send,
                                      this, message,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
  }
  void handle_send(boost::shared_ptr<std::string> /*message*/,
                   const boost::system::error_code & /*error*/,
                   std::size_t /*bytes_transferred*/)
  {
    can_send = true;
  }
};

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " <port>\n";
      return 1;
    }
    const int port = std::atoi(argv[1]);
    boost::asio::io_service io_service;
    recv_server server(io_service, port);
    io_service.run();
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}