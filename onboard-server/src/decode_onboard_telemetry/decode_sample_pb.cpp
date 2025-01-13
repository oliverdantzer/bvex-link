#include "decode_sample_pb.hpp"
#include "../sample.hpp"
#include "pb_generated/sample.pb.h"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <iostream>
#include <pb_decode.h>
#include <string>
#include <vector>

boost::shared_ptr<SampleData> decode_payload(
    boost::shared_ptr<std::vector<uint8_t>> payload, const size_t payload_size)
{
    Sample sample = Sample_init_zero;
    bool status;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(payload->data(), payload_size);

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, Sample_fields, &sample);

    /* Check for errors... */
    if(!status) {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return nullptr;
    }

    std::string metric_id(sample.metric_id);
    float timestamp = sample.timestamp;
    struct SampleMetadata metadata = {metric_id, timestamp};
    switch(sample.which_data) {
    case Sample_primitive_tag:
        switch(sample.data.primitive.which_value) {
        case Primitive_int_val_tag:
            return boost::make_shared<PrimitiveSample>(
                metadata, sample.data.primitive.value.int_val);
        case Primitive_float_val_tag:
            return boost::make_shared<PrimitiveSample>(
                metadata, sample.data.primitive.value.float_val);
        case Primitive_double_val_tag:
            return boost::make_shared<PrimitiveSample>(
                metadata, sample.data.primitive.value.double_val);
        default:
            std::cerr << "Unknown/unimplemented value type" << std::endl;
            return nullptr;
        }
    case Sample_file_tag:
        return boost::make_shared<FileSample>(
            metadata, sample.data.file.filepath, sample.data.file.extension);
    default:
        std::cerr << "Unknown/unimplemented data type" << std::endl;
        return nullptr;
    }
}