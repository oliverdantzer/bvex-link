#include "send_telemetry.h"
#include "test_file.h"
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void test_file(int socket_fd, float time) {
    char* file_path = "/home/oliver/dev/bvex-link/telemetry-uplink/tests/example_files/hello.txt";
    send_sample_file(socket_fd, "myfile", time, file_path, "txt");
    sleep(1);
}