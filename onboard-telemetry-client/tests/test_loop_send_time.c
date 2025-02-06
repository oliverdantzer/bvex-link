#include "send_telemetry.h"
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "test_loop_send_time.h"

void test_loop_send_time(int socket_fd, bool on) {
    const int n = 1000;
    double last_n_times[n];
    int i = 0;
    struct timespec start_time, end_time;
    while (1) {

        // Get the current time
        time_t t;
        time(&t);

        int temp = generate_sinusoid(20.0, 1.0 / 60.0, 0.0, (int)t) + 20.0;
        long altitude =
            generate_sinusoid(1000.0, 1.0 / 120.0, 0.0, (long)t) + 10000.0;
        float roll = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (float)t);
        double pitch = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);
        double yaw = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);

        clock_gettime(CLOCK_MONOTONIC, &start_time);
        float timestamp = (float)t;

        if(on) {
            send_sample_int32(socket_fd, "temp", timestamp, temp);
        }

        clock_gettime(CLOCK_MONOTONIC, &end_time);
        double time_taken = (end_time.tv_sec - start_time.tv_sec) + 
                            (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
        last_n_times[i++] = time_taken;

        // Calculate and print average time every n loops
        if (i == n) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) {
                sum += last_n_times[j];
            }
            double avg = sum / n;
            printf("Avg loop time: %fs, %fms, %fmicros\n", avg, avg * 1000, avg * 1000000);
            i = 0;
        }
    }
}
