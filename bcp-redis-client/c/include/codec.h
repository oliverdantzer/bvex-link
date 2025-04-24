#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque handle types
typedef struct primitive_data_t primitive_data_t;
typedef struct file_data_t file_data_t;
typedef struct sample_metadata_t sample_metadata_t;
typedef struct sample_t sample_t;

// Primitive type enum
typedef enum {
  PRIMITIVE_INT,
  PRIMITIVE_FLOAT,
  PRIMITIVE_STR,
  PRIMITIVE_BOOL
} primitive_type_t;

// Data type enum
typedef enum { DATA_TYPE_PRIMITIVE, DATA_TYPE_FILE } data_type_t;

// PrimitiveData API
primitive_data_t *primitive_data_create_int(int value);
primitive_data_t *primitive_data_create_float(double value);
primitive_data_t *primitive_data_create_str(const char *value);
primitive_data_t *primitive_data_create_bool(bool value);
void primitive_data_destroy(primitive_data_t *data);

// FileData API
file_data_t *file_data_create(const char *file_path);
void file_data_destroy(file_data_t *data);

// SampleMetadata API
sample_metadata_t *sample_metadata_create(const char *metric_id,
                                          double timestamp,
                                          data_type_t data_type);
void sample_metadata_destroy(sample_metadata_t *metadata);

// Sample API
sample_t *sample_create_with_primitive(sample_metadata_t *metadata,
                                       primitive_data_t *primitive_data);
sample_t *sample_create_with_file(sample_metadata_t *metadata,
                                  file_data_t *file_data);
void sample_destroy(sample_t *sample);

// JSON serialization/deserialization
char *sample_to_json(const sample_t *sample);
sample_t *sample_from_json(const char *json_str);

#ifdef __cplusplus
}
#endif
