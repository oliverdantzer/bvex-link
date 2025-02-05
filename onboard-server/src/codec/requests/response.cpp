#include "pb_encode.h"
#include "pb_generated/response.pb.h"
#include <cassert>
#include "../primitive.hpp"
#include <cstdint>
#include <iostream>
#include <optional>
#include <vector>

std::vector<uint8_t> encode_response(const Response& response)
{
    std::vector<uint8_t> buffer(Response_size);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer.data(), buffer.size());
    bool success = pb_encode(&stream, Response_fields, &response);
    assert(success);
    buffer.resize(stream.bytes_written);
    return buffer;
}

std::vector<uint8_t> encode_primitive_response(const std::string& metric_id,
                                               const PrimitiveValue& value)
{
    Response response = Response_init_zero;
    // metric id must be < max size
    strcpy(response.metric_id, metric_id.c_str());
    std::visit(
        [&response](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr(std::is_same_v<T, int>) {
                response.primitive.value.int_val = arg;
            } else if constexpr(std::is_same_v<T, float>) {
                response.primitive.value.float_val = arg;
            } else if constexpr(std::is_same_v<T, double>) {
                response.primitive.value.double_val = arg;
            } else {
                std::cerr << "Unknown/unimplemented response value type"
                          << std::endl;
            }
        },
        value);

    return encode_response(response);
}