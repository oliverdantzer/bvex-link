#pragma once

#include <cstddef>

constexpr size_t IPV4_HEADER_OVERHEAD = 20; // Minimum IPv4 header size in bytes
constexpr size_t UDP_HEADER_OVERHEAD = 8;   // UDP header size

// Protocol buffer overheads
constexpr size_t SAMPLE_FRAME_OVERHEAD = 40; // Sample protobuf overhead
constexpr size_t FILE_FRAME_OVERHEAD = 40; // Sample protobuf overhead
constexpr size_t PRIMITIVE_FRAME_SIZE = 40;