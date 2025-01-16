#include "encode_sample_frame.hpp"
#include "pb_generated/sample.pb.h"
#include "serialize_nanopb_struct.hpp"
#include "size_constants.hpp"
#include <cstdint>
#include <memory>
#include <pb_encode.h>
#include <string>
#include <vector>

bool data_callback(pb_ostream_t* stream, const pb_field_t* field,
                   void* const* arg)
{
    const std::vector<pb_byte_t>* data =
        static_cast<const std::vector<pb_byte_t>*>(*arg);
    if(!pb_encode_tag_for_field(stream, field)) {
        return false;
    }
    return pb_encode_string(stream, data->data(), data->size());
}

std::unique_ptr<std::vector<uint8_t>> encode_sample_frame(SampleFrameData data)
{
    SampleFrame sample_frame = SampleFrame_init_zero;
    strcpy(sample_frame.metric_id, data.metadata.metric_id.c_str());
    sample_frame.sample_id = data.sample_id;
    sample_frame.timestamp = data.metadata.timestamp;
    strcpy(sample_frame.data_type, data.data_type.c_str());
    if(data.file_extension.has_value()) {
        strcpy(sample_frame.extension, data.file_extension.value().c_str());
        sample_frame.has_extension = true;
    } else {
        sample_frame.has_extension = false;
    }
    Segment segment = Segment_init_zero;
    segment.sequence_num = data.seqnum;
    segment.num_segments = data.num_segments;
    segment.data.funcs.encode = &data_callback;
    segment.data.arg = &data.data;
    sample_frame.segment = segment;
    sample_frame.has_segment = true;
    return serialize_nanopb_struct(SampleFrame_fields, &sample_frame,
                                   SAMPLE_FRAME_OVERHEAD + data.data->size());
}
