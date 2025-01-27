#pragma once

#include "../sample.hpp"
#include "pb_generated/sample.pb.h"
#include "size_constants.hpp"
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct SampleFrameData {
    SampleMetadata metadata;
    std::string data_type;
    unsigned int sample_id;
    unsigned int num_segments;
    unsigned int seqnum;
    std::unique_ptr<std::vector<uint8_t>> data;
};

std::unique_ptr<std::vector<uint8_t>> encode_sample_frame(SampleFrameData sample_frame_data);

size_t calculate_segment_encoding_size(size_t data_size);