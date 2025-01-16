#pragma once

#include "../sample.hpp"
#include <memory>
#include <vector>
#include <cstdint>

std::unique_ptr<SampleData> decode_payload(
    std::unique_ptr<std::vector<uint8_t>> payload, size_t payload_size);
