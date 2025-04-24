#include "codec.hpp"
#include "base64.hpp"

namespace sample {

WhichPrimitive from_value(const PrimitiveType &value) {
  if (std::holds_alternative<bool>(value)) {
    return WhichPrimitive::BOOL;
  } else if (std::holds_alternative<int>(value)) {
    return WhichPrimitive::INT;
  } else if (std::holds_alternative<double>(value)) {
    return WhichPrimitive::FLOAT;
  } else if (std::holds_alternative<std::string>(value)) {
    return WhichPrimitive::STR;
  } else {
    throw std::runtime_error("Invalid primitive type");
  }
}

PrimitiveData PrimitiveData::from_value(const PrimitiveType &value) {
  return PrimitiveData(sample::from_value(value), value);
}

FileData FileData::from_file_path(const std::string &file_path) {
  std::filesystem::path path(file_path);
  std::string filename = path.filename().string();

  std::ifstream file(file_path, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + file_path);
  }

  std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

  return FileData(filename, data);
}

// JSON conversion implementations
void to_json(json &j, const WhichPrimitive &p) {
  switch (p) {
  case WhichPrimitive::INT:
    j = "int";
    break;
  case WhichPrimitive::FLOAT:
    j = "float";
    break;
  case WhichPrimitive::STR:
    j = "str";
    break;
  case WhichPrimitive::BOOL:
    j = "bool";
    break;
  }
}

void from_json(const json &j, WhichPrimitive &p) {
  std::string type = j.get<std::string>();
  if (type == "int") {
    p = WhichPrimitive::INT;
  } else if (type == "float") {
    p = WhichPrimitive::FLOAT;
  } else if (type == "str") {
    p = WhichPrimitive::STR;
  } else if (type == "bool") {
    p = WhichPrimitive::BOOL;
  } else {
    throw std::runtime_error("Invalid primitive type: " + type);
  }
}

void to_json(json &j, const WhichDataType &t) {
  switch (t) {
  case WhichDataType::PRIMITIVE:
    j = "primitive";
    break;
  case WhichDataType::FILE:
    j = "file";
    break;
  }
}

void from_json(const json &j, WhichDataType &t) {
  std::string type = j.get<std::string>();
  if (type == "primitive") {
    t = WhichDataType::PRIMITIVE;
  } else if (type == "file") {
    t = WhichDataType::FILE;
  } else {
    throw std::runtime_error("Invalid data type: " + type);
  }
}

} // namespace sample
