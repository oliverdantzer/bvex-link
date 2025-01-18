#pragma once

#include "../sample.hpp"
#include <cstdint>
#include <memory>
#include <vector>

std::unique_ptr<SampleData> decode_payload(
    std::unique_ptr<std::vector<uint8_t>> payload);
