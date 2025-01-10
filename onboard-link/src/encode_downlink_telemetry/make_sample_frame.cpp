#include "make_sample_frame.hpp"
#include "pb_generated/sample.pb.h"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <pb_encode.h>
#include <string>

bool data_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
    const std::vector<pb_byte_t> *data = static_cast<const std::vector<pb_byte_t> *>(*arg);
    if (!pb_encode_tag_for_field(stream, field)) {
        return false;
    }
    return pb_encode_string(stream, data->data(), data->size());
}

SampleFrame make_sample_frame(SampleFrameData data)
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
    return sample_frame;
}

boost::shared_ptr<std::vector<uint8_t>> serialize_sample_frame(SampleFrame sample)
{
    std::vector<uint8_t> buffer(2048); // Allocate buffer with appropriate size
    pb_ostream_t stream = pb_ostream_from_buffer(buffer.data(), buffer.size());
    if(!pb_encode(&stream, SampleFrame_fields, &sample)) {
        fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return nullptr;
    }
    buffer.resize(stream.bytes_written); // Resize buffer to actual size
    return boost::make_shared<std::vector<uint8_t>>(buffer);
}
