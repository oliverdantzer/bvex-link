#pragma once

#include "encode_downlink_telemetry/encode_primitive.hpp"
#include <cstdint>
#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

typedef std::string MetricId;

struct SampleMetadata {
    MetricId metric_id; // Should be unique to each metric
    double timestamp;   // Time since last epoch
};

class SampleData
{
  public:
    const SampleMetadata metadata;
    const std::string type;
    SampleData(SampleMetadata metadata) : metadata(metadata) {}
    virtual ~SampleData() = default;
    virtual std::unique_ptr<std::vector<uint8_t>> encode_data() = 0;
};

class PrimitiveSample : public SampleData
{
  public:
    const PrimitiveValue value;
    const std::string type = "primitive";
    PrimitiveSample(SampleMetadata metadata, PrimitiveValue value)
        : SampleData(metadata), value(value)
    {
    }
    std::unique_ptr<std::vector<uint8_t>> encode_data() override;
};

class FileSample : public SampleData
{
  public:
    const std::string file_path;
    const std::string file_extension;
    const std::string type = "file";
    FileSample(SampleMetadata metadata, std::string file_path,
               std::string file_extension)
        : SampleData(metadata), file_path(file_path),
          file_extension(file_extension)
    {
    }

    std::unique_ptr<std::vector<uint8_t>> encode_data() override;
};