#pragma once

#include "../sample.hpp"
#include "pb_generated/sample.pb.h"
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <memory>

struct SampleFrameData {
    SampleMetadata metadata;
    std::string data_type;
    std::optional<std::string> file_extension;
    unsigned int sample_id;
    unsigned int num_segments;
    unsigned int seqnum;
    std::unique_ptr<std::vector<uint8_t>> data;
};

std::unique_ptr<std::vector<uint8_t>> encode_sample_frame(SampleFrameData data);