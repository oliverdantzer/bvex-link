#include "serialize_segment.hpp"
#include "encode_primitive.hpp"
#include "encode_sample_frame.hpp"
#include "pb_generated/sample.pb.h"
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

std::unique_ptr<std::vector<uint8_t>> serialize_file_segment(
    FileEncodingInfo encoding_info,
    std::unique_ptr<std::vector<uint8_t>> file_data_segment)
{
    SampleFrameData segment_data = {.metadata = encoding_info.metadata,
                                    .data_type = "filedata",
                                    .file_extension = encoding_info.extension,
                                    .sample_id = encoding_info.sample_id,
                                    .num_segments = encoding_info.num_segments,
                                    .seqnum = encoding_info.seqnum,
                                    .data = std::move(file_data_segment)};
    return encode_sample_frame(std::move(segment_data));
}

std::unique_ptr<std::vector<uint8_t>> serialize_primitive(
    PrimitiveEncodingInfo encoding_info, PrimitiveValue value)
{
    SampleFrameData segment_data = {.metadata = encoding_info.metadata,
                                    .data_type = "primitive",
                                    .file_extension = std::nullopt,
                                    .sample_id = encoding_info.sample_id,
                                    .num_segments = 1,
                                    .seqnum = 0,
                                    .data = encode_primitive(value)};
    return encode_sample_frame(std::move(segment_data));
}