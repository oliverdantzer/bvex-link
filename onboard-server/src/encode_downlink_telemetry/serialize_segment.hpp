#include "../sample.hpp"
#include "size_constants.hpp"
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct SegmentData {
    SampleMetadata metadata;
    uint32_t sample_id;
    uint32_t num_segments;
    uint32_t seqnum;
    boost::shared_ptr<std::vector<uint8_t>> data;
};

boost::shared_ptr<std::vector<uint8_t>> serialize_file_segment(
    SegmentData data, std::string extension);

boost::shared_ptr<std::vector<uint8_t>> serialize_segment(SegmentData data);

size_t calculate_segment_serialized_size(size_t data_size)
{
    return SAMPLE_FRAME_OVERHEAD + data_size;
}