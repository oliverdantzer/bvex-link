#include "send_telemetry.h"
#include <time.h>
#include <math.h>
#include <unistd.h>


// Function to generate a sine wave value
double generate_sinusoid(double amplitude, double freq, double shift, double x)
{
    return amplitude * sin(freq * x + shift);
}

void loop(int socket_fd) {
    while(1) {
        // Get the current time
        time_t t;
        time(&t);

        int temp = generate_sinusoid(20.0, 1.0 / 60.0, 0.0, (int)t) + 20.0;
        long altitude =
            generate_sinusoid(1000.0, 1.0 / 120.0, 0.0, (long)t) + 10000.0;
        float roll = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (float)t);
        double pitch = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);
        double yaw = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);

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