#include "encode_primitive.hpp"
#include "size_constants.hpp"
#include <cstdint>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

std::unique_ptr<std::vector<uint8_t>> encode_primitive(
    const PrimitiveValue value)
{
    json primitive_frame;
    std::visit(
        [&primitive_frame](auto&& arg) { primitive_frame["value"] = arg; },
        value);
    std::string primitive_frame_string = primitive_frame.dump();
    return std::make_unique<std::vector<uint8_t>>(
        primitive_frame_string.begin(), primitive_frame_string.end());
}