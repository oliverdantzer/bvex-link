#include "command_server.h"
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if(argc < 2) {
        printf("Usage: %s <port> - Start a example command server on specified "
               "port\n",
               argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    if(port <= 0 || port > 65535) {
        printf("Error: invalid port number\n");
        return 1;
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0) {
        printf("Failed to create socket\n");
        return 1;
    }
    int commands_fd = open("/dev/null", O_WRONLY);
    if(commands_fd < 0) {
        printf("Failed to open /dev/null\n");
        return 1;
    }
    command_server_t* server =
        command_server_create(socket_fd, commands_fd, port);
    command_server_listen(server);
    while(1) {
        struct pollfd pfd = {.fd = commands_fd, .events = POLLIN};

        if(poll(&pfd, 1, 1000) > 0 && (pfd.revents & POLLIN)) {
            char* cmd = command_server_recv(server);
            if(cmd) {
                if(strcmp(cmd, "ping") == 0) {
                    command_server_broadcast(server, "pong!");
                } else if(strcmp(cmd, "time") == 0) {
                    time_t now = time(NULL);
                    char time_str[64];
                    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S",
                             localtime(&now));
                    command_server_broadcast(server, time_str);
                } else {
                    // Default help message
                    command_server_broadcast(
                        server, "Available commands:\n"
                                "  ping - Server responds with 'pong!'\n"
                                "  time - Get current server time\n"
                                "  help - Show this message");
                }
                free(cmd);
            }
        }
    }

    return 0;
}