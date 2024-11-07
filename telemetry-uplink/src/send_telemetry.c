#include "send_telemetry.h"
#include "generated/nanopb/sample.pb.h"
#include <pb_encode.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> // send()
#include <pthread.h>
#include <stdlib.h>

typedef enum {
    INT32_TYPE,
    INT64_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    BOOL_TYPE,
    STRING_TYPE,
    BYTES_TYPE
} ValueType;

typedef struct {
    int socket_fd;
    Sample sample;
} send_sample_data_t;

int send_sample(int socket_fd, Sample message)
{
    uint8_t buffer[128];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    pb_encode(&stream, Sample_fields, &message);
    send(socket_fd, buffer, stream.bytes_written, 0);
}

void* send_sample_one_arg(void* arg) {
    send_sample_data_t* data = (send_sample_data_t*)arg;
    send_sample(data->socket_fd, data->sample);
    free(data);  // Free the allocated data after sending
    return NULL;
}


Sample build_message(char* metric_id, float timestamp, void* value, ValueType type) {
    Sample message = Sample_init_zero;
    strcpy(message.metric_id, metric_id);
    message.timestamp = timestamp;

    switch(type) {
    case INT32_TYPE:
        message.which_value = Sample_int_val_tag;
        message.value.int_val = *(int32_t*)value;
        break;
    case INT64_TYPE:
        message.which_value = Sample_long_val_tag;
        message.value.long_val = *(int64_t*)value;
        break;
    case FLOAT_TYPE:
        message.which_value = Sample_float_val_tag;
        message.value.float_val = *(float*)value;
        break;
    case DOUBLE_TYPE:
        message.which_value = Sample_double_val_tag;
        message.value.double_val = *(double*)value;
        break;
    case BOOL_TYPE:
        message.which_value = Sample_bool_val_tag;
        message.value.bool_val = *(bool*)value;
        break;
    case STRING_TYPE:
        message.which_value = Sample_string_val_tag;
        strcpy(message.value.string_val, (char*)value);
        break;
    }
    return message;
}

int send_sample_async(int socket_fd, char* metric_id, float timestamp, void* value,
                ValueType type) {
    Sample message = build_message(metric_id, timestamp, value, type);
    send_sample_data_t* temp_data = malloc(sizeof(send_sample_data_t));
    if (temp_data == NULL) {
        fprintf(stderr, "Memory allocation failed for temp_data.\n");
        return 1;
    }
    temp_data->socket_fd = socket_fd;
    temp_data->sample = message;
    pthread_t temp_thread;
    int rc = pthread_create(&temp_thread, NULL, send_sample_one_arg, temp_data);
    if (rc != 0) {
        fprintf(stderr, "Failed to create temp_thread: %d\n", rc);
        free(temp_data);  // Free temp_data if thread creation fails
        return 1;
    }
    pthread_detach(temp_thread);  // Detach thread to let it run independently
}

void send_sample_int32(int socket_fd, char* metric_id, float timestamp,
                       int32_t value)
{
    send_sample_async(socket_fd, metric_id, timestamp, &value, INT32_TYPE);
}

void send_sample_int64(int socket_fd, char* metric_id, float timestamp,
                       int64_t value)
{
    send_sample_async(socket_fd, metric_id, timestamp, &value, INT64_TYPE);
}

void send_sample_float(int socket_fd, char* metric_id, float timestamp,
                       float value)
{
    send_sample_async(socket_fd, metric_id, timestamp, &value, FLOAT_TYPE);
}

void send_sample_double(int socket_fd, char* metric_id, float timestamp,
                        double value)
{
    send_sample_async(socket_fd, metric_id, timestamp, &value, DOUBLE_TYPE);
}

void send_sample_bool(int socket_fd, char* metric_id, float timestamp,
                      bool value)
{
    send_sample_async(socket_fd, metric_id, timestamp, &value, BOOL_TYPE);
}

void send_sample_string(int socket_fd, char* metric_id, float timestamp,
                        char* value)
{
    send_sample_async(socket_fd, metric_id, timestamp, value, STRING_TYPE);
}