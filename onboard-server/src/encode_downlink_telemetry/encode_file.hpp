#pragma once

#include <cstdint>
#include <vector>

std::vector<uint8_t> encode_file(const std::string& file_path,
                                 const std::string& extension);