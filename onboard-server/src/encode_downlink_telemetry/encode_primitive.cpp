#include "encode_primitive.hpp"
#include "pb_generated/primitive.pb.h"
#include "serialize_nanopb_struct.hpp"
#include "size_constants.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

std::unique_ptr<std::vector<uint8_t>> encode_primitive(
    const PrimitiveValue value)
{
    PrimitiveFrame primitive = PrimitiveFrame_init_zero;
    if(auto int_val = std::get_if<int32_t>(&value)) {
        primitive.which_value = PrimitiveFrame_int_val_tag;
        primitive.value.int_val = *int_val;
    } else if(auto long_val = std::get_if<int64_t>(&value)) {
        primitive.which_value = PrimitiveFrame_long_val_tag;
        primitive.value.long_val = *long_val;
    } else if(auto float_val = std::get_if<float>(&value)) {
        primitive.which_value = PrimitiveFrame_float_val_tag;
        primitive.value.float_val = *float_val;
    } else if(auto double_val = std::get_if<double>(&value)) {
        primitive.which_value = PrimitiveFrame_double_val_tag;
        primitive.value.double_val = *double_val;
    } else if(auto bool_val = std::get_if<bool>(&value)) {
        primitive.which_value = PrimitiveFrame_bool_val_tag;
        primitive.value.bool_val = *bool_val;
    } else if(auto string_val = std::get_if<std::string>(&value)) {
        primitive.which_value = PrimitiveFrame_string_val_tag;
        strcpy(primitive.value.string_val, string_val->c_str());
    }

    return serialize_nanopb_struct(
        &PrimitiveFrame_msg, &primitive, PRIMITIVE_FRAME_SIZE);
}