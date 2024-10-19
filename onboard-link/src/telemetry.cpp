#include "telemetry.hpp"
#include <cmath>
#include <ctime>
#include <string>

using json = nlohmann::json;

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

std::string Sample::to_json()
{
    json j;
    j["name"] = name;
    j["timestamp"] = timestamp;
    j["value"] = value;
    return j.dump();
}

Telemetry::Telemetry() {
    return;
}

std::string Telemetry::pop(size_t max_size) {
    return get_temp_reading().to_json();
}