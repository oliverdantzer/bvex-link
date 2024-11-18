#include "sample.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SampleData::SampleData(std::string metric_id, float timestamp,
                       SampleValue value)
    : metadata(SampleMetadata{metric_id, timestamp}), value(value)
{
};

std::string SampleData::to_json(int sample_id)
{
    json j;
    j["metadata"]["metric_id"] = metadata.metric_id;
    j["metadata"]["timestamp"] = metadata.timestamp;
    j["metadata"]["sample_id"] = sample_id;

    std::visit([&j](auto&& arg) { j["value"] = arg; }, value);

    return j.dump();
}

std::vector<uint8_t> SampleData::to_bytes(int sample_id) {
    std::string json = to_json(sample_id);
    std::vector<uint8_t> bytes(json.begin(), json.end());
    return bytes;
}

size_t SampleData::get_size() {
    return to_json(0).size();
}