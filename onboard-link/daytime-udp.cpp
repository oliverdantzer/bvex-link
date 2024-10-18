#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class udp_server
{
public:
  // constructor
  udp_server(boost::asio::io_service &io_service, int port, int buffer_size = 4096)
      // initialize members
      : socket_(io_service, udp::endpoint(udp::v4(), port)),
        recv_buffer_(buffer_size) // initialize buffer with buffer_size bytes
  {
    while (true)
    {
      receive();
    }
  }

private:
  udp::socket socket_;
  udp::endpoint remote_endpoint_;

  // Each char corresponds to a byte
  std::vector<char> recv_buffer_;
  void receive()
  {
    // data is written into recv_buffer_, data length and error
    // codes are passed to handle_receive

    // remote_endpoint_ recieves the endpoint of the sender
    // in async_receive_from

    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&udp_server::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  void handle_receive(const boost::system::error_code &error,
                      std::size_t packet_size /*bytes_transferred*/)
  {
    // We will get boost::asio::error::message_size
    // if the message is too big to fit in the buffer
    if (!error)
    {
      // char *received_data(recv_buffer_.data());
      // If we don't do a shared ptr, message might be destroyed
      // before the async send is done
      boost::shared_ptr<std::string> message(
          new std::string(recv_buffer_.data(), packet_size));

      socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                            boost::bind(&udp_server::handle_send,
                                        this, message,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      std::cerr << "Error code" << error.to_string() << "on receive msg: " << error.message() << std::endl;
    }
  }

  void handle_send(boost::shared_ptr<std::string> /*message*/,
                   const boost::system::error_code & /*error*/,
                   std::size_t /*bytes_transferred*/)
  {
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
    udp_server server(io_service, port);
    io_service.run();
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}