#pragma once

#include <cstdint>
#include <memory>
#include <pb_encode.h>
#include <vector>

// buffer s.t. [data, data + size) is a valid range
struct ByteBuffer {
    uint8_t* data;
    size_t size;
};

bool encode_bytes_nanopb_callback(pb_ostream_t* stream, const pb_field_t* field,
                                  void* const* arg);

/**
 * @brief Serializes a given Nanopb structure into a byte buffer.
 *
 * This function encodes a single protocol buffers message from a C structure
 * into a stream. The actual struct pointed to by src_struct must match the
 * description in fields. All required fields in the struct are assumed to have
 * been filled in.
 *
 * @param fields Pointer to the message description (fields) of the structure.
 * @param src_struct Pointer to the source structure to be serialized.
 * @param buffer_size Size of the buffer to allocate for serialization.
 * @return A vector of bytes containing the serialized data.
 * @throws std::runtime_error if encoding fails.
 *
 * Example usage:
 *    MyMessage msg = {};
 *    msg.field1 = 42;
 *    auto serialized_data = serialize_nanopb_struct(MyMessage_fields, &msg,
 * 64);
 */
std::unique_ptr<std::vector<uint8_t>> serialize_nanopb_struct(
    const pb_msgdesc_t* fields, const void* src_struct, size_t buffer_size);