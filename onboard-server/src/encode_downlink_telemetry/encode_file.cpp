#include "pb_generated/file.pb.h"
#include "serialize_nanopb_struct.hpp"
#include "size_constants.hpp"
#include "encode_file.hpp"
#include <cstdint>
#include <fstream>
#include <iterator>
#include <memory>
#include <vector>

std::unique_ptr<std::vector<uint8_t>> encode_file(std::string file_path,
                                                  std::string extension)
{
    FileFrame file_frame = FileFrame_init_zero;
    strcpy(file_frame.extension, extension.c_str());

    std::vector<uint8_t> file_data;

    std::ifstream file(file_path, std::ios::binary);
    if(file) {
        file.unsetf(std::ios::skipws);
        file_data.insert(file_data.begin(),
                         std::istream_iterator<uint8_t>(file),
                         std::istream_iterator<uint8_t>());
    }
    ByteBuffer segment_data = {
        .data = file_data.data(),
        .size = file_data.size()};
    file_frame.data.funcs.encode = &encode_bytes_nanopb_callback;
    file_frame.data.arg = &segment_data;

    return serialize_nanopb_struct(&FileFrame_msg, &file_frame,
                                   FILE_FRAME_OVERHEAD + file_data.size());
}