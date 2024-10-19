#ifndef TEMP_READING_H
#define TEMP_READING_H

#include <string>
#include <ctime>
#include <nlohmann/json.hpp>

class Sample
{
public:
    std::string name;
    std::time_t timestamp;
    std::string value;

    Sample(const std::string &name, std::time_t timestamp, const std::string &value);
    std::string to_json();
};

class Telemetry
{
public:
    Telemetry();
    std::string pop(size_t max_size);
};

Sample get_temp_reading();

#endif // TEMP_READING_H