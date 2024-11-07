#include "send_telemetry.h"
#include "generated/nanopb/sample.pb.h"
#include <pb_encode.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> // send()

typedef enum {
    INT32_TYPE,
    INT64_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    BOOL_TYPE,
    STRING_TYPE,
    BYTES_TYPE
} ValueType;

int send_sample(int socket_fd, char* metric_id, float timestamp, void* value,
                ValueType type)
{
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

    uint8_t buffer[128];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    pb_encode(&stream, Sample_fields, &message);
    send(socket_fd, buffer, stream.bytes_written, 0);


    /* Encode the request. It is written to the socket immediately
     * through our custom stream. */
    // if(!pb_encode_delimited(&output, Sample_fields, &message)) {
    //     fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&output));
    //     return 0;
    // }
}

void send_sample_int32(int socket_fd, char* metric_id, float timestamp,
                       int32_t value)
{
    send_sample(socket_fd, metric_id, timestamp, &value, INT32_TYPE);
}

void send_sample_int64(int socket_fd, char* metric_id, float timestamp,
                       int64_t value)
{
    send_sample(socket_fd, metric_id, timestamp, &value, INT64_TYPE);
}

void send_sample_float(int socket_fd, char* metric_id, float timestamp,
                       float value)
{
    send_sample(socket_fd, metric_id, timestamp, &value, FLOAT_TYPE);
}

void send_sample_double(int socket_fd, char* metric_id, float timestamp,
                        double value)
{
    send_sample(socket_fd, metric_id, timestamp, &value, DOUBLE_TYPE);
}

void send_sample_bool(int socket_fd, char* metric_id, float timestamp,
                      bool value)
{
    send_sample(socket_fd, metric_id, timestamp, &value, BOOL_TYPE);
}

void send_sample_string(int socket_fd, char* metric_id, float timestamp,
                        char* value)
{
    send_sample(socket_fd, metric_id, timestamp, value, STRING_TYPE);
}

void send_sample_bytes(int socket_fd, char* metric_id, float timestamp,
                       uint8_t* value)
{
    send_sample(socket_fd, metric_id, timestamp, value, BYTES_TYPE);
}