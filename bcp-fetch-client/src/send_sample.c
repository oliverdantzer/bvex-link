#include "send_sample.h"
#include "generated/nanopb/sample.pb.h"
#include <arpa/inet.h> // send()
#include <errno.h>
#include <pb_encode.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sample.h"

typedef struct {
    int socket_fd;
    Sample sample;
} send_sample_data_t;

static bool validate_string(const char* str, size_t max_size) {
    if (str == NULL || str[0] == '\0' || strlen(str) >= max_size) {
        return false;
    }
    return true;
}

send_status_t send_sample(int socket_fd, const Sample message)
{
    uint8_t buffer[SAMPLE_PB_H_MAX_SIZE];
    ssize_t bytes_written = encode_sample(buffer, sizeof(buffer), message);
    if(bytes_written < 0) {
        return SEND_STATUS_ENCODING_ERROR;
    }
    ssize_t bytes_sent = send(socket_fd, buffer, bytes_written, 0);
    if(bytes_sent < 0) {
#ifdef DEBUG
        fprintf(stderr, "send failed: %s\n", strerror(errno));
#endif
        return SEND_STATUS_SEND_ERROR;
    }
    return 0;
}

send_status_t send_sample_int32(int socket_fd, const char* metric_id,
                                float timestamp, int32_t value)
{
    if (!validate_string(metric_id, METRIC_ID_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    Sample sample = sample_int32(metric_id, timestamp, value);
    return send_sample(socket_fd, sample);
}

send_status_t send_sample_int64(int socket_fd, const char* metric_id,
                                float timestamp, int64_t value)
{
    if (!validate_string(metric_id, METRIC_ID_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    Sample sample = sample_int64(metric_id, timestamp, value);
    return send_sample(socket_fd, sample);
}

send_status_t send_sample_float(int socket_fd, const char* metric_id,
                                float timestamp, float value)
{
    if (!validate_string(metric_id, METRIC_ID_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    Sample sample = sample_float(metric_id, timestamp, value);
    return send_sample(socket_fd, sample);
}

send_status_t send_sample_double(int socket_fd, const char* metric_id,
                                 float timestamp, double value)
{
    if (!validate_string(metric_id, METRIC_ID_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    Sample sample = sample_double(metric_id, timestamp, value);
    return send_sample(socket_fd, sample);
}

send_status_t send_sample_bool(int socket_fd, const char* metric_id,
                               float timestamp, bool value)
{
    if (!validate_string(metric_id, METRIC_ID_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    Sample sample = sample_bool(metric_id, timestamp, value);
    return send_sample(socket_fd, sample);
}

send_status_t send_sample_string(int socket_fd, const char* metric_id,
                                 float timestamp, const char* value)
{
    if (!validate_string(metric_id, METRIC_ID_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    if (!validate_string(value, STRING_VALUE_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    Sample sample = sample_string(metric_id, timestamp, value);
    return send_sample(socket_fd, sample);
}

send_status_t send_sample_file(int socket_fd, const char* metric_id,
                               float timestamp, const char* filepath,
                               const char* extension)
{
    if (!validate_string(metric_id, METRIC_ID_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    if (!validate_string(filepath, FILE_PATH_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    if (!validate_string(extension, EXTENSION_MAX_SIZE)) {
        return SEND_STATUS_INVALID_PARAMETER;
    }
    Sample sample = sample_file(metric_id, timestamp, filepath, extension);
    return send_sample(socket_fd, sample);
}