#include "serialize_nanopb_struct.hpp"
#include <cstdint>
#include <memory>
#include <pb_encode.h>
#include <stdexcept>
#include <vector>

bool encode_bytes_nanopb_callback(pb_ostream_t* stream,
                                  const pb_field_iter_t* field,
                                  void* const* arg)
{
    if(arg == nullptr) {
        return false;
    }
    const ByteBuffer* data =
        static_cast<ByteBuffer*>(*arg);
    if(!pb_encode_tag_for_field(stream, field)) {
        return false;
    }
    return pb_encode_string(stream, data->data, data->size);
}

std::unique_ptr<std::vector<uint8_t>> serialize_nanopb_struct(
    const pb_msgdesc_t* fields, const void* src_struct, size_t buffer_size)
{
    auto buffer = std::make_unique<std::vector<uint8_t>>(
        buffer_size); // Allocate buffer with appropriate size
    pb_ostream_t stream =
        pb_ostream_from_buffer(buffer->data(), buffer->size());
    if(!pb_encode(&stream, fields, src_struct)) {
        throw std::runtime_error("Encoding failed: " +
                                 std::string(PB_GET_ERROR(&stream)));
    }
    buffer->resize(stream.bytes_written); // Resize buffer to actual size
    return buffer;
}