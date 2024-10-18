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
  udp_server(boost::asio::io_service &io_service, int port)
      : socket_(io_service, udp::endpoint(udp::v4(), port))
  {
    start_receive();
  }

private:
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  boost::array<char, 1> recv_buffer_;
  void start_receive()
  {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&udp_server::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  void handle_receive(const boost::system::error_code &error,
                      std::size_t /*bytes_transferred*/)
  {
    if (!error)
    {
      boost::shared_ptr<std::string> message(
          new std::string(make_daytime_string()));

      socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                            boost::bind(&udp_server::handle_send,
                                        this, message,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));

      start_receive();
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