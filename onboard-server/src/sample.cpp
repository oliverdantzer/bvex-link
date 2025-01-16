#include "sample.hpp"
#include "encode_downlink_telemetry/encode_file.hpp"
#include "encode_downlink_telemetry/encode_primitive.hpp"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <iterator>
#include <memory>

std::unique_ptr<std::vector<uint8_t>> PrimitiveSample::encode_data()
{
    return encode_primitive(value);
};

std::unique_ptr<std::vector<uint8_t>> FileSample::encode_data()
{
    return encode_file(file_path, file_extension);
};