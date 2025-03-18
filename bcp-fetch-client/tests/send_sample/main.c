#include "connected_udp_socket.h"
#include "send_sample.h"
#include "test_loop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SAMPLE_SERVER_ADDR "127.0.0.1"
#define SAMPLE_SERVER_PORT "3000"

static void print_usage(const char* program)
{
    printf("Usage: %s <command> [args]\n\n"
           "Commands:\n"
           "  loop              Run continuous telemetry loop\n"
           "  loop-timing      Run telemetry loop with timing measurements\n"
           "  send <type> <id> <value> [ext]\n"
           "    Send a single sample where:\n"
           "    type:  float, string, or file\n"
           "    id:    metric identifier\n"
           "    value: sample value (or filepath for file type)\n"
           "    ext:   file extension (required for file type only)\n",
           program);
}

int main(int argc, char** argv)
{
    if(argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    int socket_fd =
        connected_udp_socket(SAMPLE_SERVER_ADDR, SAMPLE_SERVER_PORT);
    if(socket_fd < 0) {
        printf("Failed to connect to server\n");
        return 1;
    }

    const char* cmd = argv[1];
    float timestamp = (float)time(NULL);

    if(strcmp(cmd, "loop") == 0) {
        run_telemetry_loop(socket_fd, false);
    } else if(strcmp(cmd, "loop-timing") == 0) {
        run_telemetry_loop(socket_fd, true);
    } else if(strcmp(cmd, "send") == 0) {
        if(argc < 5) {
            printf("Error: 'send' requires type, id, and value arguments\n");
            close(socket_fd);
            return 1;
        }

        const char* type = argv[2];
        const char* id = argv[3];
        const char* value = argv[4];

        if(strcmp(type, "float") == 0) {
            send_sample_float(socket_fd, id, timestamp, strtof(value, NULL));
        } else if(strcmp(type, "string") == 0) {
            send_sample_string(socket_fd, id, timestamp, value);
        } else if(strcmp(type, "file") == 0) {
            if(argc < 6) {
                printf("Error: file type requires extension argument\n");
                close(socket_fd);
                return 1;
            }
            send_sample_file(socket_fd, id, timestamp, value, argv[5]);
        } else {
            printf("Error: unknown type '%s'\n", type);
            close(socket_fd);
            return 1;
        }
        sleep(1); // Allow time for sample to be sent
    } else {
        printf("Unknown command: %s\n", cmd);
        close(socket_fd);
        return 1;
    }

    close(socket_fd);
    return 0;
}