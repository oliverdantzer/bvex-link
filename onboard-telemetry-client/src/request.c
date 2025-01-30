#include "request.h"
#include "generated/nanopb/request.pb.h"
#include "generated/nanopb/response.pb.h"
#include "socket.h"
#include <arpa/inet.h> // send()
#include <errno.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int socket_fd;
    char* metric_id;
} Requester;

typedef struct {
    char* metric_id;
    char* node;
    char* service;
} MakeRequesterArgs;

Requester make_requester(MakeRequesterArgs args)
{
    int sock = make_connected_send_socket(args.node, args.service);
    if(sock < 0) {
        fprintf(stderr, "Socket creation failed\n");
        return (Requester){.socket_fd = -1, .metric_id = NULL};
    }

    Requester reqr;
    reqr.socket_fd = sock;
    reqr.metric_id = args.metric_id;
    return reqr;
}

int send_request(int socket_fd, Request message)
{
    uint8_t buffer[REQUEST_PB_H_MAX_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if(!pb_encode(&stream, Request_fields, &message)) {
        fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return ERR_ENCODING_FAILED;
    }
    ssize_t bytes_sent = send(socket_fd, buffer, stream.bytes_written, 0);
    if(bytes_sent == -1) {
        fprintf(stderr, "send failed: %s\n", strerror(errno));
        return ERR_SEND_FAILED;
    }
    return 0;
}

int recv_response(int socket_fd, Response response)
{
    char recv_buffer[RESPONSE_PB_H_MAX_SIZE];
    ssize_t bytes_received =
        recv(socket_fd, recv_buffer, RESPONSE_PB_H_MAX_SIZE, 0);
    if(bytes_received == -1) {
        fprintf(stderr, "recv failed: %s\n", strerror(errno));
        return ERR_RECV_FAILED;
    } else if(bytes_received == 0) {
        printf("Connection closed by the peer\n");
        return ERR_RECV_FAILED;
    }

    bool status;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(recv_buffer, bytes_received);

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, Response_fields, &response);

    /* Check for errors... */
    if(!status) {
        fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return ERR_DECODING_FAILED;
    }
    return 0;
}

typedef struct {
    int err;
    Response response;
} RequestResult;

RequestResult request(Requester reqr)
{
    RequestResult result;

    Request request = Request_init_zero;
    strcpy(request.metric_id, reqr.metric_id);
    int send_err = send_request(reqr.socket_fd, request);
    if(send_err) {
        result.err = send_err;
        return result;
    }
    Response response = Response_init_zero;
    int recv_err = recv_response(reqr.socket_fd, response);
    if(recv_err) {
        result.err = recv_err;
        return result;
    }
    result.response = response;
    return result;
}

typedef struct {
    int err;
    int value;
} RequestIntResult;

RequestIntResult request_int(Requester reqr)
{
    RequestIntResult result;

    RequestResult request_result = request(reqr);
    if(request_result.err) {
        result.err = request_result.err;
    }

    if(request_result.response.primitive.which_value != Primitive_int_val_tag) {
        fprintf(stderr, "Invalid response type\n");
        result.err = ERR_INVALID_RESPONSE_TYPE;
        return result;
    }

    result.value = request_result.response.primitive.value.int_val;
    return result;
}