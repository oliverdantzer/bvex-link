#include "test_loop.h"
#include "data.h"
#include "sample_sender.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void run_telemetry_loop(const char* node, const char* service,
                        bool measure_timing)
{
    const int TIMING_WINDOW = 1000;
    double timing_buffer[TIMING_WINDOW];
    int timing_index = 0;
    struct timespec start_time, end_time;

    // Create senders for each metric
    sample_sender_status_t status;
    sample_sender_params_t temp_params = {
        .metric_id = "temperature", .node = node, .service = service};
    sample_sender_params_t roll_params = {
        .metric_id = "roll", .node = node, .service = service};

    sample_sender_int32_t* temp_sender =
        make_sample_sender_int32(temp_params, &status);
    if(status != SAMPLE_SENDER_STATUS_OK) {
        printf("Failed to create temperature sender\n");
        return;
    }

    sample_sender_float_t* roll_sender =
        make_sample_sender_float(roll_params, &status);
    if(status != SAMPLE_SENDER_STATUS_OK) {
        printf("Failed to create roll sender\n");
        destroy_sample_sender((sample_sender_t*)temp_sender);
        return;
    }

    while(1) {
        if(measure_timing) {
            clock_gettime(CLOCK_MONOTONIC, &start_time);
        }

        // Get current time and generate simulated data
        time_t t;
        time(&t);
        float timestamp = (float)t;

        // Generate and send simulated sensor data
        int temp = (int)generate_sinusoid(20.0, 1.0 / 60.0, 0.0, t) + 20;
        float roll = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, t);

        send_int32(temp_sender, timestamp, temp);
        send_float(roll_sender, timestamp, roll);

        if(measure_timing) {
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            double time_taken = (end_time.tv_sec - start_time.tv_sec) +
                                (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

            timing_buffer[timing_index++] = time_taken;

            if(timing_index == TIMING_WINDOW) {
                double sum = 0.0;
                for(int i = 0; i < TIMING_WINDOW; i++) {
                    sum += timing_buffer[i];
                }
                double avg = sum / TIMING_WINDOW;
                printf("Avg send time: %.3fms\n", avg * 1000.0);
                timing_index = 0;
            }
        }

        usleep(1000); // 1ms delay
    }

    // Cleanup
    destroy_sample_sender((sample_sender_t*)temp_sender);
    destroy_sample_sender((sample_sender_t*)roll_sender);
}