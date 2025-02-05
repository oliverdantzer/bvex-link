#include "decode.hpp"
#include "pb_generated/sample.pb.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <pb_decode.h>
#include <string>
#include <vector>

// returning ptr to SampleData is necessary here because
// it allows us to use polymorphism and return classes
// derived from the abstract class SampleData
std::optional<Sample> decode_payload(
    std::vector<uint8_t> payload)
{
    Sample sample = Sample_init_zero;
    bool status;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream =
        pb_istream_from_buffer(payload.data(), payload.size());

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, Sample_fields, &sample);

    /* Check for errors... */
    if(!status) {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return std::nullopt;
    }

    return sample;
}