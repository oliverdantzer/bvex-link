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

#define SAMPLE_SERVER_ADDR "localhost"
#define SAMPLE_SERVER_PORT "3000"

int main(int argc, char** argv)
{
    if(argc < 2) {
        printf(
            "Usage: %s target_name target_port test_name [additional_args]\n",
            argv[0]);
        printf("  test_name: The name of the test to run (test_spam, test_one, "
               "test_loop).\n");
        printf("  additional_args: Additional arguments required by specific "
               "tests.\n");
        printf("    - test_spam: Requires an additional argument specifying "
               "the number of times to send.\n");
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
    if(strcmp(test_name, "test_spam") == 0) {
        if(argc != 5) {
            printf("Usage for test_spam: %s target_name target_port test_spam "
                   "times\n",
                   argv[0]);
            return 1;
        }
        int times = atoi(argv[2]);
        int socket_fd =
            connected_udp_socket(SAMPLE_SERVER_ADDR, SAMPLE_SERVER_PORT);
        n_samples(times, socket_fd);
        close(socket_fd);
    } else if(strcmp(test_name, "sample") == 0) {
        char* type = argv[2];
        if(strcmp(type, "float") == 0) {
            float value = strtof(argv[3], NULL);
            int socket_fd =
                connected_udp_socket(SAMPLE_SERVER_ADDR, SAMPLE_SERVER_PORT);
            test_one_float(socket_fd, start_time_in_seconds, value);
            close(socket_fd);
        } else if(strcmp(type, "string") == 0) {
            char* value = argv[3];
            int socket_fd =
                connected_udp_socket(SAMPLE_SERVER_ADDR, SAMPLE_SERVER_PORT);
            test_one_string(socket_fd, start_time_in_seconds, value);
            close(socket_fd);
        }
    } else if(strcmp(test_name, "request") == 0) {
        char* type = argv[2];
        if(strcmp(type, "float") == 0) {
            test_request_float("test");
        } else if(strcmp(type, "string") == 0) {
            test_request_string("test");
        }
    } else if(strcmp(test_name, "test_loop") == 0) {
        int socket_fd =
            connected_udp_socket(SAMPLE_SERVER_ADDR, SAMPLE_SERVER_PORT);
        primitive_telemetry_loop(socket_fd);
        close(socket_fd);
    } else if(strcmp(test_name, "test_file") == 0) {
        int socket_fd =
            connected_udp_socket(SAMPLE_SERVER_ADDR, SAMPLE_SERVER_PORT);
        test_file(socket_fd, start_time_in_seconds);
        close(socket_fd);
    } else {
        printf("Unknown test name: %s\n", test_name);
    }
    return 0;
}