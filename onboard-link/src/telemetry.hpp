#ifndef TEMP_READING_H
#define TEMP_READING_H

#include "command.hpp"
#include <string>

class Sample
{
  public:
    std::string name;
    std::time_t timestamp;
    std::string value;

    Sample(const std::string& name, std::time_t timestamp,
           const std::string& value);
    std::string to_json();
};

class Telemetry
{
  public:
    Telemetry(Command& command);

    /**
     * @brief Retrieves the next telemetry data as a JSON string.
     *
     * This function pops the next telemetry data from the internal queue
     * and returns it as a JSON-formatted string. The max size of the
     * data is limited to this.command_.get_max_packet_size()
     *
     * @return std::string The next telemetry data in JSON format.
     */
    std::string pop();
};

Sample get_temp_reading();

#endif // TEMP_READING_H