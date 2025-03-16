#include "send_sample.h"
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

send_status_t send_sample(int socket_fd, Sample message)
{
    uint8_t buffer[SAMPLE_PB_H_MAX_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if(!pb_encode(&stream, Sample_fields, &message)) {
#ifdef DEBUG
        fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
#endif
        return SEND_STATUS_ENCODING_ERROR;
    }

    ssize_t bytes_sent = send(socket_fd, buffer, stream.bytes_written, 0);
    if(bytes_sent < 0) {
#ifdef DEBUG
        fprintf(stderr, "send failed: %s\n", strerror(errno));
#endif
        return SEND_STATUS_SEND_ERROR;
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

send_status_t send_sample_async(int socket_fd, Sample sample)
{
    send_sample_data_t* temp_data = malloc(sizeof(send_sample_data_t));
    if(temp_data == NULL) {
#ifdef DEBUG
        fprintf(stderr, "Memory allocation failed for temp_data.\n");
#endif
        return SEND_STATUS_MEMORY_ALLOCATION_ERROR;
    }
    temp_data->socket_fd = socket_fd;
    temp_data->sample = sample;
    pthread_t temp_thread;
    int rc = pthread_create(&temp_thread, NULL, send_sample_one_arg, temp_data);
    if(rc != 0) {
#ifdef DEBUG
        fprintf(stderr, "Failed to create temp_thread: %d\n", rc);
#endif
        free(temp_data); // Free temp_data if thread creation fails
        return SEND_STATUS_THREAD_CREATION_ERROR;
    }
    pthread_detach(temp_thread); // Detach thread to let it run independently
    return SEND_STATUS_OK;
}

send_status_t send_sample_int32(int socket_fd, char* metric_id, float timestamp,
                                int32_t value)
{
#ifdef BCP_FETCH_BOUNDS_CHECKING
    if(strlen(metric_id) >= sizeof(((Sample*)0)->metric_id)) {
        return BOUNDS_CHECK_ERROR;
    }
#endif
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_int_val_tag;
    sample.data.primitive.value.int_val = value;
    return send_sample_async(socket_fd, sample);
}

send_status_t send_sample_int64(int socket_fd, char* metric_id, float timestamp,
                                int64_t value)
{
#ifdef BCP_FETCH_BOUNDS_CHECKING
    if(strlen(metric_id) >= sizeof(((Sample*)0)->metric_id)) {
        return BOUNDS_CHECK_ERROR;
    }
#endif
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_long_val_tag;
    sample.data.primitive.value.long_val = value;
    return send_sample_async(socket_fd, sample);
}

send_status_t send_sample_float(int socket_fd, char* metric_id, float timestamp,
                                float value)
{
#ifdef BCP_FETCH_BOUNDS_CHECKING
    if(strlen(metric_id) >= sizeof(((Sample*)0)->metric_id)) {
        return BOUNDS_CHECK_ERROR;
    }
#endif
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_float_val_tag;
    sample.data.primitive.value.float_val = value;
    return send_sample_async(socket_fd, sample);
}

send_status_t send_sample_double(int socket_fd, char* metric_id,
                                 float timestamp, double value)
{
#ifdef BCP_FETCH_BOUNDS_CHECKING
    if(strlen(metric_id) >= sizeof(((Sample*)0)->metric_id)) {
        return BOUNDS_CHECK_ERROR;
    }
#endif
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_double_val_tag;
    sample.data.primitive.value.double_val = value;
    return send_sample_async(socket_fd, sample);
}

send_status_t send_sample_bool(int socket_fd, char* metric_id, float timestamp,
                               bool value)
{
#ifdef BCP_FETCH_BOUNDS_CHECKING
    if(strlen(metric_id) >= sizeof(((Sample*)0)->metric_id)) {
        return BOUNDS_CHECK_ERROR;
    }
#endif
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_bool_val_tag;
    sample.data.primitive.value.bool_val = value;
    return send_sample_async(socket_fd, sample);
}

send_status_t send_sample_string(int socket_fd, char* metric_id,
                                 float timestamp, char* value)
{
#ifdef BCP_FETCH_BOUNDS_CHECKING
    if(strlen(metric_id) >= sizeof(((Sample*)0)->metric_id) ||
       strlen(value) >= sizeof(((Sample*)0)->data.primitive.value.string_val)) {
        return BOUNDS_CHECK_ERROR;
    }
#endif
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_primitive_tag;
    sample.data.primitive.which_value = primitive_Primitive_string_val_tag;
    strcpy(sample.data.primitive.value.string_val, value);
    return send_sample_async(socket_fd, sample);
}

send_status_t send_sample_file(int socket_fd, char* metric_id, float timestamp,
                               char* filepath, char* extension)
{
#ifdef BCP_FETCH_BOUNDS_CHECKING
    if(strlen(metric_id) >= sizeof(((Sample*)0)->metric_id) ||
       strlen(filepath) >= sizeof(((Sample*)0)->data.file.filepath) ||
       strlen(extension) >= sizeof(((Sample*)0)->data.file.extension)) {
        return BOUNDS_CHECK_ERROR;
    }
#endif
    Sample sample = Sample_init_zero;
    strcpy(sample.metric_id, metric_id);
    sample.timestamp = timestamp;
    sample.which_data = Sample_file_tag;
    strcpy(sample.data.file.filepath, filepath);
    strcpy(sample.data.file.extension, extension);
    return send_sample_async(socket_fd, sample);
}