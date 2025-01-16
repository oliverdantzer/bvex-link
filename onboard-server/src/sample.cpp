#include "sample.hpp"
#include "encode_downlink_telemetry/encode_primitive.hpp"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <iterator>
#include <memory>

std::unique_ptr<std::vector<uint8_t>> PrimitiveSample::data_serialized()
{
    return encode_primitive(this->value);
}

std::unique_ptr<std::vector<uint8_t>> FileSample::data_serialized()
{
    std::unique_ptr<std::vector<uint8_t>> data =
        std::make_unique<std::vector<uint8_t>>();
    std::ifstream file(this->file_path, std::ios::binary);

    if(file) {
        file.unsetf(std::ios::skipws);
        data->insert(data->begin(), std::istream_iterator<uint8_t>(file),
                     std::istream_iterator<uint8_t>());
    }

    return data;
}