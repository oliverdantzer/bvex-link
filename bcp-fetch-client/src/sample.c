#include "sample.h"
#include "generated/nanopb/sample.pb.h"
#include <errno.h>
#include <pb_encode.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>

ssize_t encode_sample(uint8_t* buffer, size_t buffer_size, const Sample sample)
{
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);
    if(!pb_encode(&stream, Sample_fields, &sample)) {
#ifdef DEBUG
        fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
#endif
        return -1;
    }
    return stream.bytes_written;
}

Sample sample_int32(const char* metric_id, float timestamp, int32_t value)
{
    assert(metric_id && *metric_id != 0);
    assert(strnlen(metric_id, METRIC_ID_MAX_SIZE) < METRIC_ID_MAX_SIZE);
    Sample sample = Sample_init_zero;
    strlcpy(sample.metric_id, metric_id, METRIC_ID_MAX_SIZE);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_int_val_tag;
    sample.data.primitive.value.int_val = value;
    return sample;
}

Sample sample_int64(const char* metric_id, float timestamp, int64_t value)
{
    assert(metric_id && *metric_id != 0);
    assert(strnlen(metric_id, METRIC_ID_MAX_SIZE) < METRIC_ID_MAX_SIZE);
    Sample sample = Sample_init_zero;
    strlcpy(sample.metric_id, metric_id, METRIC_ID_MAX_SIZE);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_long_val_tag;
    sample.data.primitive.value.long_val = value;
    return sample;
}

Sample sample_float(const char* metric_id, float timestamp, float value)
{
    assert(metric_id && *metric_id != 0);
    assert(strnlen(metric_id, METRIC_ID_MAX_SIZE) < METRIC_ID_MAX_SIZE);
    Sample sample = Sample_init_zero;
    strlcpy(sample.metric_id, metric_id, METRIC_ID_MAX_SIZE);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_float_val_tag;
    sample.data.primitive.value.float_val = value;
    return sample;
}

Sample sample_double(const char* metric_id, float timestamp, double value)
{
    assert(metric_id && *metric_id != 0);
    assert(strnlen(metric_id, METRIC_ID_MAX_SIZE) < METRIC_ID_MAX_SIZE);
    Sample sample = Sample_init_zero;
    strlcpy(sample.metric_id, metric_id, METRIC_ID_MAX_SIZE);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_double_val_tag;
    sample.data.primitive.value.double_val = value;
    return sample;
}

Sample sample_bool(const char* metric_id, float timestamp, bool value)
{
    assert(metric_id && *metric_id != 0);
    assert(strnlen(metric_id, METRIC_ID_MAX_SIZE) < METRIC_ID_MAX_SIZE);
    Sample sample = Sample_init_zero;
    strlcpy(sample.metric_id, metric_id, METRIC_ID_MAX_SIZE);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_bool_val_tag;
    sample.data.primitive.value.bool_val = value;
    return sample;
}

Sample sample_string(const char* metric_id, float timestamp, const char* value)
{
    assert(metric_id && *metric_id != 0);
    assert(value && *value != 0);
    assert(strnlen(metric_id, METRIC_ID_MAX_SIZE) < METRIC_ID_MAX_SIZE);
    assert(strnlen(value, STRING_VALUE_MAX_SIZE) < STRING_VALUE_MAX_SIZE);
    Sample sample = Sample_init_zero;
    strlcpy(sample.metric_id, metric_id, METRIC_ID_MAX_SIZE);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_string_val_tag;
    strlcpy(sample.data.primitive.value.string_val, value, STRING_VALUE_MAX_SIZE);
    return sample;
}

Sample sample_file(const char* metric_id, float timestamp, const char* filepath, const char* extension)
{
    assert(metric_id && *metric_id != 0);
    assert(filepath && *filepath != 0);
    assert(extension && *extension != 0);
    assert(strnlen(metric_id, METRIC_ID_MAX_SIZE) < METRIC_ID_MAX_SIZE);
    assert(strnlen(filepath, FILE_PATH_MAX_SIZE) < FILE_PATH_MAX_SIZE);
    assert(strnlen(extension, EXTENSION_MAX_SIZE) < EXTENSION_MAX_SIZE);
    Sample sample = Sample_init_zero;
    strlcpy(sample.metric_id, metric_id, METRIC_ID_MAX_SIZE);
    sample.timestamp = timestamp;
    sample.which_data = Sample_file_tag;
    strlcpy(sample.data.file.filepath, filepath, FILE_PATH_MAX_SIZE);
    strlcpy(sample.data.file.extension, extension, EXTENSION_MAX_SIZE);
    return sample;
}