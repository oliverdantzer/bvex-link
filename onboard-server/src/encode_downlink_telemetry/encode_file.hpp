#pragma once

#include <cstdint>
#include <memory>
#include <vector>

std::unique_ptr<std::vector<uint8_t>> encode_file(std::string file_path,
                                                  std::string extension);