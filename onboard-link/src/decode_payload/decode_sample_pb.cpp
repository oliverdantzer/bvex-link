#include "decode_sample_pb.hpp"
#include "pb_generated/sample.pb.h"
#include "../sample.hpp"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <pb_decode.h>
#include <string>
#include <vector>
#include <cstdint>

boost::shared_ptr<SampleData> decode_payload(
    boost::shared_ptr<std::vector<uint8_t>> payload, const size_t payload_size)
{
    Sample sample = Sample_init_zero;
    bool status;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream =
        pb_istream_from_buffer(payload->data(), payload_size);

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
    case Sample_long_val_tag:
        return boost::make_shared<SampleData>(metric_id, timestamp,
                                              sample.value.long_val);
    case Sample_float_val_tag:
        return boost::make_shared<SampleData>(metric_id, timestamp,
                                               sample.value.float_val);
    case Sample_double_val_tag:
        return boost::make_shared<SampleData>(metric_id, timestamp,
                                                sample.value.double_val);
    case Sample_bool_val_tag:
        return boost::make_shared<SampleData>(metric_id, timestamp,
                                              sample.value.bool_val);
    case Sample_string_val_tag:
        return boost::make_shared<SampleData>(metric_id, timestamp,
                                                sample.value.string_val);
    default:
        printf("Unknown value type\n");
        return nullptr;
    }

    // switch(sample.which_value) {
    // case Sample_int_val_tag:
    //     return boost::make_shared<IntSample>(metric_id, timestamp,
    //                                          sample.value.int_val);
    // case Sample_long_val_tag:
    //     return boost::make_shared<LongSample>(metric_id, timestamp,
    //                                           sample.value.long_val);
    // case Sample_float_val_tag:
    //     return boost::make_shared<FloatSample>(metric_id, timestamp,
    //                                            sample.value.float_val);
    // case Sample_double_val_tag:
    //     return boost::make_shared<DoubleSample>(metric_id, timestamp,
    //                                             sample.value.double_val);
    // case Sample_bool_val_tag:
    //     return boost::make_shared<BoolSample>(metric_id, timestamp,
    //                                           sample.value.bool_val);
    // case Sample_string_val_tag:
    //     return boost::make_shared<StringSample>(metric_id, timestamp,
    //                                             sample.value.string_val);
    // default:
    //     printf("Unknown value type\n");
    //     return nullptr;
    // }
}