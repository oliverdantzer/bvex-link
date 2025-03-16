#include "test_request.h"
#include "request_sample.h"
#include "stdio.h"

#define REQUEST_SERVER_ADDR "localhost"
#define REQUEST_SERVER_PORT "8080"

void test_request_string(char* metric_id)
{
    Requester requester =
        make_requester(metric_id, REQUEST_SERVER_ADDR, REQUEST_SERVER_PORT);
    if(requester.socket_fd < 0) {
        printf("Socket creation failed.\n");
        return;
    }
    RequestStringResult result = request_string(&requester);
    if(result.err) {
        printf("Request failed.\n");
        return;
    }
    printf("Request successful. Result: %s.\n", result.value);
}

void test_request_float(char* metric_id)
{
    Requester requester =
        make_requester(metric_id, REQUEST_SERVER_ADDR, REQUEST_SERVER_PORT);
    if(requester.socket_fd < 0) {
        printf("Socket creation failed.\n");
        return;
    }
    RequestFloatResult result = request_float(&requester);
    if(result.err) {
        printf("Request failed.\n");
        return;
    }
    printf("Request successful. Result: %f.\n", result.value);
}