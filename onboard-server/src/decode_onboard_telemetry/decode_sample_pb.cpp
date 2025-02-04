#include "decode_sample_pb.hpp"
#include "../sample.hpp"
#include "pb_generated/sample.pb.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <pb_decode.h>
#include <string>
#include <vector>

// returning ptr to SampleData is necessary here because
// it allows us to use polymorphism and return classes
// derived from the abstract class SampleData
std::unique_ptr<SampleData> decode_payload(
    std::unique_ptr<std::vector<uint8_t>> payload)
{
    Sample sample = Sample_init_zero;
    bool status;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream =
        pb_istream_from_buffer(payload->data(), payload->size());

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, Sample_fields, &sample);

    /* Check for errors... */
    if(!status) {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return nullptr;
    }

    std::string metric_id(sample.metric_id);
    float timestamp = sample.timestamp;
    SampleMetadata metadata = {.metric_id = metric_id, .timestamp = timestamp};
    switch(sample.which_data) {
    case Sample_primitive_tag:
        switch(sample.data.primitive.which_value) {
        case Primitive_int_val_tag:
            return std::make_unique<PrimitiveSample>(
                metadata, sample.data.primitive.value.int_val);
        case Primitive_float_val_tag:
            return std::make_unique<PrimitiveSample>(
                metadata, sample.data.primitive.value.float_val);
        case Primitive_double_val_tag:
            return std::make_unique<PrimitiveSample>(
                metadata, sample.data.primitive.value.double_val);
        default:
            std::cerr << "Unknown/unimplemented value type" << std::endl;
            return nullptr;
        }
    case Sample_file_tag:
        return std::make_unique<FileSample>(metadata, sample.data.file.filepath,
                                            sample.data.file.extension);
    default:
        std::cerr << "Unknown/unimplemented data type" << std::endl;
        return nullptr;
    }
}