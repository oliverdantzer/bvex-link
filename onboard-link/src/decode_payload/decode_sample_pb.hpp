#pragma once

#include "../sample.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <cstdint>

boost::shared_ptr<SampleData> decode_payload(
    boost::shared_ptr<std::vector<uint8_t>> payload, size_t message_length);

int hi(int hie);