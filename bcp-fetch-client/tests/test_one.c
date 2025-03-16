#include "send_sample.h"
#include "test_one.h"
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void test_one_float(int socket_fd, float time, float value) {

    send_sample_float(socket_fd, "test", time, value);
    // pthread_join(send_thread, NULL); // Wait for the thread to finish
    sleep(1);
}

void test_one_string(int socket_fd, float time, char* value) {
    send_sample_string(socket_fd, "test", time, value);
    sleep(1);
}