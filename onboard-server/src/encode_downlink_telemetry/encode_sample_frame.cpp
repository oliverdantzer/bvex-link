#include "encode_sample_frame.hpp"
#include "pb_generated/sample.pb.h"
#include "serialize_nanopb_struct.hpp"
#include "size_constants.hpp"
#include <cstdint>
#include <memory>
#include <pb_encode.h>
#include <string>
#include <vector>

std::unique_ptr<std::vector<uint8_t>> encode_sample_frame(SampleFrameData data)
{
    SampleFrame sample_frame = SampleFrame_init_zero;
    strcpy(sample_frame.metric_id, data.metadata.metric_id.c_str());
    sample_frame.sample_id = data.sample_id;
    sample_frame.timestamp = data.metadata.timestamp;
    strcpy(sample_frame.data_type, data.data_type.c_str());
    Segment segment = Segment_init_zero;
    segment.sequence_num = data.seqnum;
    segment.num_segments = data.num_segments;
    segment.data.funcs.encode = &encode_bytes_nanopb_callback;
    segment.data.arg = &data.data;
    sample_frame.segment = segment;
    sample_frame.has_segment = true;
    return serialize_nanopb_struct(&SampleFrame_msg, &sample_frame,
                                   SAMPLE_FRAME_OVERHEAD + data.data->size());
}

size_t calculate_segment_encoding_size(size_t data_size)
{
    return SAMPLE_FRAME_OVERHEAD + data_size;
}