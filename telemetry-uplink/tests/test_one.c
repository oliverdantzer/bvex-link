#include "send_telemetry.h"
#include "test_one.h"
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void test_one(int socket_fd, float time) {

    send_sample_float(socket_fd, "test", time, 0.0f);
    // pthread_join(send_thread, NULL); // Wait for the thread to finish
    sleep(1);
}