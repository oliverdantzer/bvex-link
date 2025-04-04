#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>
#include "generated/nanopb/sample.pb.h"

#define STRING_VALUE_MAX_SIZE 4096
#define METRIC_ID_MAX_SIZE 64
#define FILE_PATH_MAX_SIZE 128
#define EXTENSION_MAX_SIZE 16

ssize_t encode_sample(uint8_t* buffer, size_t buffer_size, const Sample sample);

/**
 * @brief Creates an int32_t sample.
 *
 * @param metric_id The identifier for the metric. Must be non-null, non-empty, and less than
 * METRIC_ID_MAX_SIZE.
 * @param timestamp The timestamp of the sample.
 * @param value The int32_t value to encode.
 * @return Sample structure.
 */
Sample sample_int32(const char* metric_id, float timestamp, int32_t value);

/**
 * @brief Creates an int64_t sample.
 *
 * @param metric_id The identifier for the metric. Must be non-null, non-empty, and less than
 * METRIC_ID_MAX_SIZE.
 * @param timestamp The timestamp of the sample.
 * @param value The int64_t value to encode.
 * @return Sample structure.
 */
Sample sample_int64(const char* metric_id, float timestamp, int64_t value);

/**
 * @brief Creates a float sample.
 *
 * @param metric_id The identifier for the metric. Must be non-null, non-empty, and less than
 * METRIC_ID_MAX_SIZE.
 * @param timestamp The timestamp of the sample.
 * @param value The float value to encode.
 * @return Sample structure.
 */
Sample sample_float(const char* metric_id, float timestamp, float value);

/**
 * @brief Creates a double sample.
 *
 * @param metric_id The identifier for the metric. Must be non-null, non-empty, and less than
 * METRIC_ID_MAX_SIZE.
 * @param timestamp The timestamp of the sample.
 * @param value The double value to encode.
 * @return Sample structure.
 */
Sample sample_double(const char* metric_id, float timestamp, double value);

/**
 * @brief Creates a boolean sample.
 *
 * @param metric_id The identifier for the metric. Must be non-null, non-empty, and less than
 * METRIC_ID_MAX_SIZE.
 * @param timestamp The timestamp of the sample.
 * @param value The boolean value to encode.
 * @return Sample structure.
 */
Sample sample_bool(const char* metric_id, float timestamp, bool value);

/**
 * @brief Creates a string sample.
 *
 * @note The string value must have a null terminator.
 *
 * @param metric_id The identifier for the metric. Must be non-null, non-empty, and less than
 * METRIC_ID_MAX_SIZE.
 * @param timestamp The timestamp of the sample.
 * @param value The string value to encode. Must be non-null, non-empty, and less than
 * STRING_VALUE_MAX_SIZE.
 * @return Sample structure.
 */
Sample sample_string(const char* metric_id, float timestamp, const char* value);

/**
 * @brief Creates a file sample.
 *
 * @param metric_id The identifier for the metric. Must be non-null, non-empty, and less than
 * METRIC_ID_MAX_SIZE.
 * @param timestamp The timestamp of the sample.
 * @param filepath The absolute path to the file to encode. Must be non-null, non-empty, and less than
 * FILE_PATH_MAX_SIZE.
 * @param extension The file extension. Must be non-null, non-empty, and less than EXTENSION_MAX_SIZE.
 * @return Sample structure.
 */
Sample sample_file(const char* metric_id, float timestamp, const char* filepath,
                  const char* extension);

#ifdef __cplusplus
}
#endif
