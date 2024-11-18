#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <variant>
#include <vector>

typedef std::string MetricId;

struct SampleMetadata {
    MetricId metric_id; // Should be unique to each metric
    double timestamp;   // Time since last epoch
};

typedef std::variant<int32_t, int64_t, float, double, bool, std::string> SampleValue;

class SampleData
{
  public:
    const SampleMetadata metadata;
    const SampleValue value;
    SampleData(std::string metric_id, float timestamp,
                       SampleValue value);
    std::string to_json(int sample_id);
    std::vector<uint8_t> to_bytes(int sample_id);
    size_t get_size();
};
