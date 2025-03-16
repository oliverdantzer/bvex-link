#include "request.h"
#include "send_telemetry.h"
#include "test_file.h"
#include "test_loop.h"
#include "test_loop_send_time.h"
#include "test_n_samples.h"
#include "test_one.h"
#include "test_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SAMPLE_SERVER_ADDR "127.0.0.1"
#define SAMPLE_SERVER_PORT "3000"
#define REQUEST_SERVER_ADDR "127.0.0.1"
#define REQUEST_SERVER_PORT "8080"

int main(int argc, char** argv)
{
    if(argc < 2) {
        printf(
            "Usage: %s target_name target_port test_name [additional_args]\n",
            argv[0]);
        printf("  test_name: The name of the test to run (test_spam, test_one, "
               "test_loop, sample, request).\n");
        printf("  additional_args: Additional arguments required by specific "
               "tests.\n");
        printf("    - test_spam: Requires an additional argument specifying "
               "the number of times to send.\n");
        printf("    - sample: Requires type (float, string, or file) and value "
               "arguments.\n");
        printf(
            "      For file type, value should be filepath and extension.\n");
        return 1;
    }
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC,
                  &start_time); // Get start time with nanosecond precision

    // Convert timespec to a float representing the time in seconds
    float start_time_in_seconds =
        start_time.tv_sec + (start_time.tv_nsec / 1e9);

    printf("Start time: %f\n", start_time_in_seconds);

    char* test_name = argv[1];
    char* metric_id = argv[2];
    if(strcmp(test_name, "sample") == 0) {
        int socket_fd =
            connected_udp_socket(SAMPLE_SERVER_ADDR, SAMPLE_SERVER_PORT);
        if(socket_fd < 0) {
            printf("Socket creation failed.\n");
            return 1;
        }
        char* type = argv[3];
        if(strcmp(type, "float") == 0) {
            float value = strtof(argv[4], NULL);
            send_sample_float(socket_fd, metric_id, start_time_in_seconds,
                              value);
        } else if(strcmp(type, "string") == 0) {
            char* value = argv[4];
            send_sample_string(socket_fd, metric_id, start_time_in_seconds,
                               value);
        } else if(strcmp(type, "file") == 0) {
            if(argc < 6) {
                printf("Error: file type requires filepath and extension "
                       "arguments.\n");
                close(socket_fd);
                return 1;
            }
            char* filepath = argv[4];
            char* extension = argv[5];
            send_sample_file(socket_fd, metric_id, start_time_in_seconds,
                             filepath, extension);
        }
        sleep(1);
        close(socket_fd);
    } else if(strcmp(test_name, "request") == 0) {
        Requester requester =
            make_requester(metric_id, REQUEST_SERVER_ADDR, REQUEST_SERVER_PORT);
        if(requester.socket_fd < 0) {
            printf("Socket creation failed.\n");
            return 1;
        }
        char* type = argv[3];
        if(strcmp(type, "float") == 0) {
            RequestFloatResult result = request_float(&requester);
            if(result.err) {
                printf("Request failed.\n");
                return 1;
            }
            printf("Request successful. Result: %f.\n", result.value);
        } else if(strcmp(type, "string") == 0) {
            RequestStringResult result = request_string(&requester);
            if(result.err) {
                printf("Request failed.\n");
                return 1;
            }
            printf("Request successful. Result: %s.\n", result.value);
        }
    } else {
        printf("Unknown test name: %s\n", test_name);
    }
    return 0;
}