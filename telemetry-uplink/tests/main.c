#include "socket.h"
#include "test_loop.h"
#include "test_loop_send_time.h"
#include "test_n_samples.h"
#include "test_one.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if(argc < 4) {
        printf(
            "Usage: %s target_name target_port test_name [additional_args]\n",
            argv[0]);
        printf("  target_name: The name or IP address of the target.\n");
        printf("  target_port: The port number of the target service.\n");
        printf("  test_name: The name of the test to run (test_spam, test_one, "
               "test_loop).\n");
        printf("  additional_args: Additional arguments required by specific "
               "tests.\n");
        printf("    - test_spam: Requires an additional argument specifying "
               "the number of times to send.\n");
        return 1;
    }
    char* target_name = argv[1];
    char* target_service = argv[2];
    char* test_name = argv[3];
    int socket_fd = make_connected_send_socket(target_name, target_service);
    if(strcmp(test_name, "test_spam") == 0) {
        if(argc != 5) {
            printf("Usage for test_spam: %s target_name target_port test_spam "
                   "times\n",
                   argv[0]);
            return 1;
        }
        int times = atoi(argv[4]);
        n_samples(times, socket_fd);
    } else if(strcmp(test_name, "test_one") == 0) {
        struct timespec start_time;
        clock_gettime(CLOCK_MONOTONIC,
                      &start_time); // Get start time with nanosecond precision

        // Convert timespec to a float representing the time in seconds
        float start_time_in_seconds =
            start_time.tv_sec + (start_time.tv_nsec / 1e9);

        printf("Start time: %f\n", start_time_in_seconds);

        test_one(socket_fd, start_time_in_seconds);
    } else if(strcmp(test_name, "test_loop") == 0) {
        primitive_telemetry_loop(socket_fd);
    } else {
        printf("Unknown test name: %s\n", test_name);
    }
    close(socket_fd);
    return 0;
}