#include "send_sample.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "test_n_samples.h"

float n_samples(int n, int socket_fd) {
    clock_t start_time = clock();
    
    for (int i = 0; i < n; i++) {
        send_sample_float(socket_fd, "test", start_time, 0.0f);
    }
    
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // in seconds
    printf("\nTime taken to send %d samples: %f seconds\n", n, time_taken);
    printf("Avg. Hz.: %f\n", n / time_taken);
    printf("Avg. time per send: %f microseconds\n", (time_taken / n) * 1000000);
    sleep(1);
    return time_taken;
}