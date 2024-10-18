#include "temp_reading.h"
#include <cmath>
#include <ctime>
#include <string>

Sample::Sample(const std::string &name, std::time_t timestamp, const std::string &value)
    : name(name), timestamp(timestamp), value(value) {}

Sample get_temp_reading()
{
    std::time_t t = std::time(nullptr);
    int period = 30;
    float value = 10 * std::sin((2 * M_PI * t) / period) + 20;
    Sample sample("temp", t, std::to_string(value));
    return sample;
}