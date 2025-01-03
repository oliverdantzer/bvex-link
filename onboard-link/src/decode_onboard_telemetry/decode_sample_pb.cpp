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
    switch(sample.which_value) {
    case Sample_int_val_tag:
        return boost::make_shared<SampleData>(metric_id, timestamp,
                                              sample.value.int_val);
    case Sample_float_val_tag:
        return boost::make_shared<SampleData>(metric_id, timestamp,
                                              sample.value.float_val);
    case Sample_double_val_tag:
        return boost::make_shared<SampleData>(metric_id, timestamp,
                                              sample.value.double_val);
    default:
        std::cerr << "Unknown/unimplemented value type" << std::endl;
        return nullptr;
    }
}