#include "send_telemetry.h"
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "test_loop.h"

typedef struct {
    int socket_fd;
    const char* label;
    float timestamp;
    int32_t value;
} send_sample_data_t;

// Thread function to send data asynchronously
void* send_sample_int32_thread(void* arg) {
    send_sample_data_t* data = (send_sample_data_t*)arg;
    send_sample_int32(data->socket_fd, data->label, data->timestamp, data->value);
    free(data);  // Free the allocated data after sending
    return NULL;
}

void test_pthread(int socket_fd, bool on) {
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

        // Allocate memory for temperature sample data and start a thread for sending it
        if(on) {
            send_sample_data_t* temp_data = malloc(sizeof(send_sample_data_t));
            if (temp_data == NULL) {
                fprintf(stderr, "Memory allocation failed for temp_data.\n");
                continue;  // Skip this loop iteration on failure
            }
            temp_data->socket_fd = socket_fd;
            temp_data->label = "temperature";
            temp_data->timestamp = timestamp;
            temp_data->value = temp;

            pthread_t temp_thread;
            int rc = pthread_create(&temp_thread, NULL, send_sample_int32_thread, temp_data);
            if (rc != 0) {
                fprintf(stderr, "Failed to create temp_thread: %d\n", rc);
                free(temp_data);  // Free temp_data if thread creation fails
                continue;  // Skip this loop iteration on failure
            }
            pthread_detach(temp_thread);  // Detach thread to let it run independently
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
