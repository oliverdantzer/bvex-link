#ifndef COMMAND_H
#define COMMAND_H

#include "telemetry.hpp"

struct Telecommand
{
    std::string id;
    std::chrono::system_clock::time_point time; // seconds since last epoch
    std::string name;
    std::string value;
};

class Command
{
public:
    Command(size_t bps);
    void add_tc_json(const std::string &telecommands_json);
    size_t get_bps();
    size_t get_max_packet_size();
private:
    size_t bps_;
    size_t max_packet_size_;
    std::vector<std::string> telecommands_;
};

#endif COMMAND_H