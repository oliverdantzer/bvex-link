#pragma once

#include <optional>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../sample.hpp"
#include <cstdint>
#include "pb_generated/sample.pb.h"

struct SampleFrameData {
    SampleMetadata metadata;
    std::string data_type;
    std::optional<std::string> file_extension;
    unsigned int sample_id;
    unsigned int num_segments;
    unsigned int seqnum;
    boost::shared_ptr<std::vector<uint8_t>> data;
};

SampleFrame make_sample_frame(SampleFrameData data);

boost::shared_ptr<std::vector<uint8_t>> serialize_sample_frame(SampleFrame data);