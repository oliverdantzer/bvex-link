#include "sample_sender.h"
#include "connected_udp_socket.h"
#include "sample.pb.h"
#include <arpa/inet.h>
#include <errno.h>
#include <pb_encode.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool validate_string(const char* str, size_t max_size)
{
    if(str == NULL) {
        return false;
    }

    size_t len = strlen(str);
    size_t size = len + 1;
    if(len == 0 || size > max_size) {
        return false;
    }

    return true;
}

static ssize_t encode_sample(uint8_t* buffer, size_t buffer_size,
                             const Sample sample)
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

send_sample_status_t send_sample(sample_sender_t* sender, const Sample message)
{
    uint8_t buffer[SAMPLE_PB_H_MAX_SIZE];
    ssize_t bytes_written = encode_sample(buffer, sizeof(buffer), message);
    if(bytes_written < 0) {
        return SEND_SAMPLE_STATUS_ENCODING_ERROR;
    }
    ssize_t bytes_sent = send(sender->socket_fd, buffer, bytes_written, 0);
    if(bytes_sent < 0) {
#ifdef DEBUG
        fprintf(stderr, "send failed: %s\n", strerror(errno));
#endif
        return SEND_SAMPLE_STATUS_SEND_ERROR;
    }
    return SEND_SAMPLE_STATUS_OK;
}

sample_sender_t* make_sample_sender(sample_sender_params_t params,
                                    sample_sender_status_t* status)
{
    if(!validate_string(params.metric_id, member_size(Sample, metric_id))) {
        *status = SAMPLE_SENDER_STATUS_INVALID_PARAMETER;
        return NULL;
    }
    int sock = connected_udp_socket(params.node, params.service);
    if(sock < 0) {
        *status = SAMPLE_SENDER_STATUS_SOCKET_CREATION_FAILED;
        return NULL;
    }

    sample_sender_t* sender = malloc(sizeof(sample_sender_t));
    sender->socket_fd = sock;
    sender->metric_id = params.metric_id;
    *status = SAMPLE_SENDER_STATUS_OK;
    return sender;
}

void destroy_sample_sender(sample_sender_t* sender)
{
    close(sender->socket_fd);
    free(sender);
}