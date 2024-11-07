#include "socket.h"
#include "test_loop.h"
#include "test_one.h"
#include "test_spam.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "test_pthread.h"


int main(int argc, char** argv)
{
    if(argc < 4) {
        printf("Usage: %s target_name target_port test_name\n", argv[0]);
        return 1;
    }
    char* target_name = argv[1];
    char* target_service = argv[2];
    char* test_name = argv[3];
    int socket_fd =
        make_async_connected_send_socket(target_name, target_service);
    if(strcmp(test_name, "test_one") == 0) {
        test_one(socket_fd);
    } else if(strcmp(test_name, "test_spam") == 0) {
        spam_send(atoi(argv[4]), socket_fd, "test", 0.0, 0.0);
    } else if(strcmp(test_name, "test_pthread") == 0) {
        test_pthread(socket_fd);
    } else {
        printf("Unknown test name: %s\n", test_name);
    }
    close(socket_fd);
    return 0;
}