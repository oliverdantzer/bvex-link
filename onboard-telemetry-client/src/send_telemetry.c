#include "send_telemetry.h"
#include "generated/nanopb/sample.pb.h"
#include <arpa/inet.h> // send()
#include <errno.h>
#include <pb_encode.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int socket_fd;
    Sample sample;
} send_sample_data_t;

int send_sample(int socket_fd, Sample message)
{
    uint8_t buffer[SAMPLE_PB_H_MAX_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if(!pb_encode(&stream, Sample_fields, &message)) {
        fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return -1;
    }

    ssize_t bytes_sent = send(socket_fd, buffer, stream.bytes_written, 0);
    if(bytes_sent < 0) {
        fprintf(stderr, "send failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void* send_sample_one_arg(void* arg)
{
    send_sample_data_t* data = (send_sample_data_t*)arg;
    send_sample(data->socket_fd, data->sample);
    free(data); // Free the allocated data after sending
    return NULL;
}

int send_sample_async(int socket_fd, Sample sample)
{
    send_sample_data_t* temp_data = malloc(sizeof(send_sample_data_t));
    if(temp_data == NULL) {
        fprintf(stderr, "Memory allocation failed for temp_data.\n");
        return -1;
    }
    temp_data->socket_fd = socket_fd;
    temp_data->sample = sample;
    pthread_t temp_thread;
    int rc = pthread_create(&temp_thread, NULL, send_sample_one_arg, temp_data);
    if(rc != 0) {
        fprintf(stderr, "Failed to create temp_thread: %d\n", rc);
        free(temp_data); // Free temp_data if thread creation fails
        return -1;
    }
    pthread_detach(temp_thread); // Detach thread to let it run independently
    return 0;
}

void send_sample_int32(int socket_fd, char* metric_id, float timestamp,
                       int32_t value)
{
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_int_val_tag;
    sample.data.primitive.value.int_val = value;
    send_sample_async(socket_fd, sample);
}

void send_sample_int64(int socket_fd, char* metric_id, float timestamp,
                       int64_t value)
{
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_long_val_tag;
    sample.data.primitive.value.long_val = value;
    send_sample_async(socket_fd, sample);
}

void send_sample_float(int socket_fd, char* metric_id, float timestamp,
                       float value)
{
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_float_val_tag;
    sample.data.primitive.value.float_val = value;
    send_sample_async(socket_fd, sample);
}

void send_sample_double(int socket_fd, char* metric_id, float timestamp,
                        double value)
{
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_double_val_tag;
    sample.data.primitive.value.double_val = value;
    send_sample_async(socket_fd, sample);
}

void send_sample_bool(int socket_fd, char* metric_id, float timestamp,
                      bool value)
{
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_bool_val_tag;
    sample.data.primitive.value.bool_val = value;
    send_sample_async(socket_fd, sample);
}

void send_sample_string(int socket_fd, char* metric_id, float timestamp,
                        char* value)
{
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_string_val_tag;
    strcpy(sample.data.primitive.value.string_val, value);
    send_sample_async(socket_fd, sample);
}

void send_sample_file(int socket_fd, char* metric_id, float timestamp,
                      char* filepath, char* extension)
{
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_file_tag;
    strcpy(sample.data.file.filepath, filepath);
    strcpy(sample.data.file.extension, extension);
    send_sample_async(socket_fd, sample);
}