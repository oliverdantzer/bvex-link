#pragma once

#include <cstdint>
#include <vector>

std::vector<uint8_t> encode_file(std::string file_path,
                                                  std::string extension);