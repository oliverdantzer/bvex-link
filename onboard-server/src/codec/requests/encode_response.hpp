#include "pb_generated/response.pb.h"
#include "pb_encode.h"
#include <cstdint>
#include <vector>
#include "../sample.hpp"
#include "../encode_downlink_telemetry/encode_primitive.hpp"

std::vector<uint8_t> encode_response(const Response& response);

std::vector<uint8_t> encode_primitive_response(const std::string& metric_id, const PrimitiveValue& value);