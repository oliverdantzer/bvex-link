#ifndef UDP_SEND_SERVER_H
#define UDP_SEND_SERVER_H

#include <string>
#include <boost/asio.hpp>
#include "telemetry.hpp"
#include "command.hpp"

using boost::asio::ip::udp;

boost::shared_ptr<std::string> get_message();

/*! 
   Send loop:
   \dot
   digraph example {
       node [shape=record, fontname=Helvetica, fontsize=10];
       b [ label="class B" URL="\ref B"];
       c [ label="class C" URL="\ref C"];
       b -> c [ arrowhead="open", style="dashed" ];
   }
   \enddot
 */

class send_server
{
public:
    send_server(boost::asio::io_service &io_service,
                Telemetry &telemetry,
                Command &command,
                boost::asio::ip::port_type port,
                boost::asio::ip::address& target_address,
                boost::asio::ip::port_type target_port,
                std::size_t bps);

private:
    udp::socket socket_;
    Telemetry &telemetry_;
    udp::endpoint remote_endpoint_;
    std::vector<char> recv_buffer_;
    boost::asio::steady_timer timer_;
    Command &command_;

    /**
     * @brief Sends the next telemetry data packet async, then calls handle_send() on completion.
     * 
     * Calls telemetry_.pop() to get the next telemetry data packet, then sends it async to remote_endpoint_.
     * Gives handle_send the shared_ptr to the message, the boost error code, and the number of bytes sent.
     * 
     */
    void start_send();

    /**
     * @brief Handles the completion of the async packet send called by start_send.
     * 
     * Reads out errors if there were any, then calls schedule_send().
     * 
     */
    void handle_send(boost::shared_ptr<std::string> message,
                     const boost::system::error_code &error,
                     std::size_t sent_size);

    /**
     * @brief Schedules the next call of self.start_send() to happen after OS finishes sending previous packet.
     * 
     * Calculates send time based on the number of bytes sent in the previous packet and command_.get_bps().
     * 
     * @param sent_size The number of bytes sent in the previous packet.
     */
    void schedule_send(std::size_t sent_size);
};

#endif // UDP_SEND_SERVER_H