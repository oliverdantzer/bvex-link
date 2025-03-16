#include "test_loop.h"
#include "common.h"
#include "send_sample.h"
#include <time.h>
#include <unistd.h>

void primitive_telemetry_loop(int socket_fd)
{
    while(1) {
        // Get the current time
        time_t t;
        time(&t);

        int temp =
            (int)generate_sinusoid(20.0, 1.0 / 60.0, 0.0, (double)t) + 20.0;
        long altitude =
            (int)generate_sinusoid(1000.0, 1.0 / 120.0, 0.0, (double)t) +
            10000.0;
        float roll = (int)generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);
        double pitch = (int)generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);
        double yaw = (int)generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);

        float timestamp = (float)t;

        send_sample_int32(socket_fd, "temperature", timestamp, temp);
        send_sample_int64(socket_fd, "altitude", timestamp, altitude);
        send_sample_float(socket_fd, "roll", timestamp, roll);
        send_sample_double(socket_fd, "pitch", timestamp, pitch);
        send_sample_double(socket_fd, "yaw", timestamp, yaw);

        // Sleep 1 ms to mimic high sample rate
        sleep(0.001);
    }
}