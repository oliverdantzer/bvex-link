#include "serialize_segment.hpp"
#include "make_sample_frame.hpp"
#include "pb_generated/sample.pb.h"
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct SegmentData {
    SampleMetadata metadata;
    std::optional<std::string> file_extension;
    unsigned int sample_id;
    unsigned int num_segments;
    unsigned int seqnum;
    boost::shared_ptr<std::vector<uint8_t>> data;
};

boost::shared_ptr<std::vector<uint8_t>> serialize_file_segment(
    SegmentData data, std::string extension)
{
    SampleFrameData segment_data = {.metadata = data.metadata,
                                    .data_type = "filedata",
                                    .file_extension = extension,
                                    .sample_id = data.sample_id,
                                    .num_segments = data.num_segments,
                                    .seqnum = data.seqnum,
                                    .data = data.data};
    SampleFrame sample = make_sample_frame(segment_data);
    return serialize_sample_frame(sample);
}

boost::shared_ptr<std::vector<uint8_t>> serialize_segment(SegmentData data,
                                                          std::string type)
{
    SampleFrameData segment_data = {.metadata = data.metadata,
                                    .data_type = type,
                                    .file_extension = std::nullopt,
                                    .sample_id = data.sample_id,
                                    .num_segments = data.num_segments,
                                    .seqnum = data.seqnum,
                                    .data = data.data};
    SampleFrame sample = make_sample_frame(segment_data);
    return serialize_sample_frame(sample);
}