#include "encode_sample_frame.hpp"
#include "pb_generated/sample.pb.h"
#include "serialize_nanopb_struct.hpp"
#include "size_constants.hpp"
#include <cstdint>
#include <memory>
#include <pb_encode.h>
#include <string>
#include <vector>

std::unique_ptr<std::vector<uint8_t>> encode_sample_frame(
    SampleFrameData sample_frame_data)
{
    SampleFrame sample_frame = SampleFrame_init_zero;
    strcpy(sample_frame.metric_id,
           sample_frame_data.metadata.metric_id.c_str());
    sample_frame.sample_id = sample_frame_data.sample_id;
    sample_frame.timestamp = sample_frame_data.metadata.timestamp;
    strcpy(sample_frame.data_type, sample_frame_data.data_type.c_str());
    Segment segment = Segment_init_zero;
    segment.sequence_num = sample_frame_data.seqnum;
    segment.num_segments = sample_frame_data.num_segments;
    sample_frame.segment = segment;
    sample_frame.has_segment = true;

    size_t encoded_sample_frame_size =
        SAMPLE_FRAME_OVERHEAD + sample_frame_data.data->size();
    ByteBuffer segment_data = {
        .data = sample_frame_data.data->data(),
        .size = sample_frame_data.data->size()};
    segment.data.arg = &segment_data;
    segment.data.funcs.encode = &encode_bytes_nanopb_callback;
    std::unique_ptr<std::vector<uint8_t>> data = serialize_nanopb_struct(
        &SampleFrame_msg, &sample_frame, encoded_sample_frame_size);
    return data;
}

size_t calculate_segment_encoding_size(size_t data_size)
{
    return SAMPLE_FRAME_OVERHEAD + data_size;
}