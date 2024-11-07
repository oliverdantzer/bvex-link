#include "send_telemetry.h"
#include <time.h>
#include <stdio.h>

float spam_send(int times, int socket_fd, char* metric_id, float timestamp, float value) {
    clock_t start_time = clock();
    
    for (int i = 0; i < times; i++) {
        send_sample_float(socket_fd, metric_id, timestamp, value);
    }
    
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // in seconds
    printf("Time taken to send %d samples: %f seconds\n", times, time_taken);
    printf("Avg. Hz.: %f\n", times / time_taken);
    printf("Avg. time per send: %f microseconds\n", (time_taken / times) * 1000000);
    return time_taken;
}