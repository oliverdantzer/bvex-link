#include "../sample.hpp"
#include "size_constants.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "encode_primitive.hpp"

struct FileEncodingInfo {
    SampleMetadata metadata;
    std::string extension;
    uint32_t sample_id;
    uint32_t num_segments;
    uint32_t seqnum;
};

std::unique_ptr<std::vector<uint8_t>> serialize_file_segment(
    FileEncodingInfo encoding_info,
    std::unique_ptr<std::vector<uint8_t>> file_data_segment);

struct PrimitiveEncodingInfo {
    SampleMetadata metadata;
    uint32_t sample_id;
};

std::unique_ptr<std::vector<uint8_t>> serialize_primitive(
    PrimitiveEncodingInfo encoding_info, PrimitiveValue value);

size_t calculate_file_segment_serialized_size(size_t data_size)
{
    return SAMPLE_FRAME_OVERHEAD + data_size;
}

size_t calculate_primitive_serialized_size()
{
    return SAMPLE_FRAME_OVERHEAD + PRIMITIVE_FRAME_SIZE;
}