#include "pb_generated/file.pb.h"
#include "serialize_nanopb_struct.hpp"
#include "size_constants.hpp"
#include <cstdint>
#include <fstream>
#include <iterator>
#include <memory>
#include <vector>

std::unique_ptr<std::vector<uint8_t>> encode_file(std::string file_path,
                                                  std::string extension)
{
    File file_frame = File_init_zero;
    strcpy(file_frame.extension, extension.c_str());

    std::vector<uint8_t> file_data;

    std::ifstream file(file_path, std::ios::binary);
    if(file) {
        file.unsetf(std::ios::skipws);
        file_data.insert(file_data.begin(),
                         std::istream_iterator<uint8_t>(file),
                         std::istream_iterator<uint8_t>());
    }

    file_frame.data.funcs.encode = &encode_bytes_nanopb_callback;
    file_frame.data.arg = &file_data;

    return serialize_nanopb_struct(File_fields, &file_frame,
                                   FILE_FRAME_OVERHEAD + file_data.size());
}