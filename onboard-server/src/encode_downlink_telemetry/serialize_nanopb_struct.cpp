#include <cstdint>
#include <pb_encode.h>
#include <stdexcept>
#include <vector>
#include <memory>

std::unique_ptr<std::vector<uint8_t>> serialize_nanopb_struct(
    const pb_msgdesc_t* fields, const void* src_struct, size_t buffer_size)
{
    auto buffer = std::make_unique<std::vector<uint8_t>>(
        buffer_size); // Allocate buffer with appropriate size
    pb_ostream_t stream =
        pb_ostream_from_buffer(buffer->data(), buffer->size());
    if(!pb_encode(&stream, fields, &src_struct)) {
        throw std::runtime_error("Encoding failed: " +
                                 std::string(PB_GET_ERROR(&stream)));
    }
    buffer->resize(stream.bytes_written); // Resize buffer to actual size
    return buffer;
}