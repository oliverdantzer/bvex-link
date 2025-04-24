#include "codec.h"
#include "codec.hpp"
#include <stdlib.h>
#include <string.h>

// Helper function to convert C string to C++ string
static std::string to_cpp_string(const char *str) {
  return str ? std::string(str) : std::string();
}

// Helper function to convert C++ string to C string
static char *to_c_string(const std::string &str) {
  char *c_str = (char *)malloc(str.length() + 1);
  if (c_str) {
    strcpy(c_str, str.c_str());
  }
  return c_str;
}

// PrimitiveData implementation
struct primitive_data_t {
  sample::PrimitiveData *data;
};

primitive_data_t *primitive_data_create_int(int value) {
  primitive_data_t *handle =
      (primitive_data_t *)malloc(sizeof(primitive_data_t));
  if (handle) {
    handle->data =
        new sample::PrimitiveData(sample::WhichPrimitive::INT, value);
  }
  return handle;
}

primitive_data_t *primitive_data_create_float(double value) {
  primitive_data_t *handle =
      (primitive_data_t *)malloc(sizeof(primitive_data_t));
  if (handle) {
    handle->data =
        new sample::PrimitiveData(sample::WhichPrimitive::FLOAT, value);
  }
  return handle;
}

primitive_data_t *primitive_data_create_str(const char *value) {
  primitive_data_t *handle =
      (primitive_data_t *)malloc(sizeof(primitive_data_t));
  if (handle) {
    handle->data = new sample::PrimitiveData(sample::WhichPrimitive::STR,
                                             to_cpp_string(value));
  }
  return handle;
}

primitive_data_t *primitive_data_create_bool(bool value) {
  primitive_data_t *handle =
      (primitive_data_t *)malloc(sizeof(primitive_data_t));
  if (handle) {
    handle->data =
        new sample::PrimitiveData(sample::WhichPrimitive::BOOL, value);
  }
  return handle;
}

void primitive_data_destroy(primitive_data_t *data) {
  if (data) {
    delete data->data;
    free(data);
  }
}

// FileData implementation
struct file_data_t {
  sample::FileData *data;
};

file_data_t *file_data_create(const char *file_path) {
  file_data_t *handle = (file_data_t *)malloc(sizeof(file_data_t));
  if (handle) {
    try {
      handle->data =
          new sample::FileData(sample::FileData::from_file_path(file_path));
    } catch (...) {
      free(handle);
      return NULL;
    }
  }
  return handle;
}

void file_data_destroy(file_data_t *data) {
  if (data) {
    delete data->data;
    free(data);
  }
}

// SampleMetadata implementation
struct sample_metadata_t {
  sample::SampleMetadata *metadata;
};

sample_metadata_t *sample_metadata_create(const char *metric_id,
                                          double timestamp,
                                          data_type_t data_type) {
  sample_metadata_t *handle =
      (sample_metadata_t *)malloc(sizeof(sample_metadata_t));
  if (handle) {
    sample::WhichDataType type = (data_type == DATA_TYPE_PRIMITIVE)
                                     ? sample::WhichDataType::PRIMITIVE
                                     : sample::WhichDataType::FILE;
    handle->metadata =
        new sample::SampleMetadata(to_cpp_string(metric_id), timestamp, type);
  }
  return handle;
}

void sample_metadata_destroy(sample_metadata_t *metadata) {
  if (metadata) {
    delete metadata->metadata;
    free(metadata);
  }
}

// Sample implementation
struct sample_t {
  sample::Sample *sample;
};

sample_t *sample_create_with_primitive(sample_metadata_t *metadata,
                                       primitive_data_t *primitive_data) {
  sample_t *handle = (sample_t *)malloc(sizeof(sample_t));
  if (handle) {
    handle->sample =
        new sample::Sample(*metadata->metadata, *primitive_data->data);
  }
  return handle;
}

sample_t *sample_create_with_file(sample_metadata_t *metadata,
                                  file_data_t *file_data) {
  sample_t *handle = (sample_t *)malloc(sizeof(sample_t));
  if (handle) {
    handle->sample = new sample::Sample(*metadata->metadata, *file_data->data);
  }
  return handle;
}

void sample_destroy(sample_t *sample) {
  if (sample) {
    delete sample->sample;
    free(sample);
  }
}

// JSON serialization/deserialization
char *sample_to_json(const sample_t *sample) {
  if (!sample)
    return NULL;
  try {
    sample::json j = *sample->sample;
    return to_c_string(j.dump());
  } catch (...) {
    return NULL;
  }
}

sample_t *sample_from_json(const char *json_str) {
  if (!json_str)
    return NULL;
  sample_t *handle = (sample_t *)malloc(sizeof(sample_t));
  if (handle) {
    try {
      sample::json j = sample::json::parse(json_str);
      handle->sample = new sample::Sample(j.get<sample::Sample>());
    } catch (...) {
      free(handle);
      return NULL;
    }
  }
  return handle;
}
