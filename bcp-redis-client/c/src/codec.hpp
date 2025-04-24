#pragma once

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace sample {

using json = nlohmann::json;

// Primitive types
using PrimitiveType = std::variant<int, double, std::string, bool>;

// Enum for primitive types
enum class WhichPrimitive { INT, FLOAT, STR, BOOL };

// Helper function to determine primitive type
WhichPrimitive from_value(const PrimitiveType &value);

// Custom serialization for PrimitiveType
void to_json(json &j, const PrimitiveType &p) {
  std::visit([&j](const auto &value) { j = value; }, p);
}

void from_json(const json &j, PrimitiveType &p) {
  if (j.is_number_integer()) {
    p = j.get<int>();
  } else if (j.is_number_float()) {
    p = j.get<double>();
  } else if (j.is_string()) {
    p = j.get<std::string>();
  } else if (j.is_boolean()) {
    p = j.get<bool>();
  } else {
    throw std::runtime_error("Invalid JSON type for PrimitiveType");
  }
}

// PrimitiveData class
class PrimitiveData {
public:
  WhichPrimitive which_primitive;
  PrimitiveType value;

  PrimitiveData() = default;
  PrimitiveData(WhichPrimitive which, PrimitiveType val)
      : which_primitive(which), value(val) {}

  static PrimitiveData from_value(const PrimitiveType &value);

  // JSON serialization/deserialization
  friend void to_json(json &j, const PrimitiveData &p) {
    j = json::object();
    to_json(j["which_primitive"], p.which_primitive);
    to_json(j["value"], p.value);
  }

  friend void from_json(const json &j, PrimitiveData &p) {
    from_json(j["which_primitive"], p.which_primitive);
    from_json(j["value"], p.value);
  }
};

// FileData class
class FileData {
public:
  std::string filename;
  std::vector<uint8_t> data;

  FileData() = default;
  FileData(const std::string &name, const std::vector<uint8_t> &d)
      : filename(name), data(d) {}

  static FileData from_file_path(const std::string &file_path);

  // JSON serialization/deserialization
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(FileData, filename, data)
};

// Enum for data types
enum class WhichDataType { PRIMITIVE, FILE };

// SampleMetadata class
class SampleMetadata {
public:
  std::string metric_id;
  double timestamp;
  WhichDataType which_data_type;

  SampleMetadata() = default;
  SampleMetadata(const std::string &id, double ts, WhichDataType type)
      : metric_id(id), timestamp(ts), which_data_type(type) {}

  // JSON serialization/deserialization
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SampleMetadata, metric_id, timestamp,
                                 which_data_type)
};

// Sample class
class Sample {
public:
  SampleMetadata metadata;
  std::variant<PrimitiveData, FileData> data;

  Sample() = default;
  Sample(const SampleMetadata &meta,
         const std::variant<PrimitiveData, FileData> &d)
      : metadata(meta), data(d) {}

  // JSON serialization/deserialization
  friend void to_json(json &j, const Sample &s) {
    j = json::object();
    to_json(j["metadata"], s.metadata);
    to_json(j["data"], s.data);
  }

  friend void from_json(const json &j, Sample &s) {
    from_json(j["metadata"], s.metadata);
    from_json(j["data"], s.data);
  }
};

// JSON conversion functions
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
  std::string s = j.get<std::string>();
  if (s == "int")
    p = WhichPrimitive::INT;
  else if (s == "float")
    p = WhichPrimitive::FLOAT;
  else if (s == "str")
    p = WhichPrimitive::STR;
  else if (s == "bool")
    p = WhichPrimitive::BOOL;
  else
    throw std::runtime_error("Invalid WhichPrimitive value");
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
  std::string s = j.get<std::string>();
  if (s == "primitive")
    t = WhichDataType::PRIMITIVE;
  else if (s == "file")
    t = WhichDataType::FILE;
  else
    throw std::runtime_error("Invalid WhichDataType value");
}

// Custom serialization for std::variant<PrimitiveData, FileData>
void to_json(json &j, const std::variant<PrimitiveData, FileData> &v) {
  std::visit([&j](const auto &value) { j = value; }, v);
}

void from_json(const json &j, std::variant<PrimitiveData, FileData> &v) {
  if (j.contains("which_primitive")) {
    v = j.get<PrimitiveData>();
  } else if (j.contains("filename")) {
    v = j.get<FileData>();
  } else {
    throw std::runtime_error(
        "Invalid JSON for variant<PrimitiveData, FileData>");
  }
}

} // namespace sample
