#include "command_server.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// Thread function for handling server operations
static void* server_thread(void* arg)
{
    command_server_t* server = (command_server_t*)arg;
    struct pollfd* fds =
        malloc((server->max_connections + 1) * sizeof(struct pollfd));
    if(!fds) {
        return NULL;
    }

    // Setup poll for server socket
    fds[0].fd = server->socket_fd;
    fds[0].events = POLLIN;

    while(server->running) {
        // Setup poll fds for all connections
        size_t nfds = 1; // Start with 1 for server socket
        for(size_t i = 0; i < server->num_connections; i++) {
            fds[nfds].fd = server->connections[i].fd;
            fds[nfds].events = POLLIN;
            nfds++;
        }

        // Poll for events
        int ret = poll(fds, nfds, 1000); // 1 second timeout
        if(ret < 0) {
            if(errno == EINTR)
                continue;
            break;
        }

        // Check server socket for new connections
        if(fds[0].revents & POLLIN) {
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int client_fd = accept(server->socket_fd,
                                   (struct sockaddr*)&client_addr, &addr_len);

            if(client_fd >= 0 &&
               server->num_connections < server->max_connections) {
                // Add new connection
                server->connections[server->num_connections].fd = client_fd;
                server->connections[server->num_connections].addr = client_addr;
                server->num_connections++;
            } else if(client_fd >= 0) {
                // Too many connections, reject
                close(client_fd);
            }
        }

        // Check client connections for data
        for(size_t i = 0; i < server->num_connections; i++) {
            if(fds[i + 1].revents & POLLIN) {
                char buffer[BUFFER_SIZE];
                ssize_t bytes =
                    read(server->connections[i].fd, buffer, BUFFER_SIZE - 1);

                if(bytes > 0) {
                    buffer[bytes] = '\0';
                    // Write to commands fd with newline
                    write(server->commands_fd, buffer, bytes);
                    write(server->commands_fd, "\n", 1);
                } else if(bytes <= 0) {
                    // Connection closed or error
                    close(server->connections[i].fd);
                    // Remove connection by shifting array
                    memmove(&server->connections[i],
                            &server->connections[i + 1],
                            (server->num_connections - i - 1) *
                                sizeof(client_connection_t));
                    server->num_connections--;
                    i--; // Adjust index since we removed an element
                }
            }
        }
    }

    free(fds);
    return NULL;
}

command_server_t* command_server_create(int socket_fd, int commands_fd,
                                        size_t max_connections)
{
    command_server_t* server = malloc(sizeof(command_server_t));
    if(!server) {
        return NULL;
    }

    server->connections = malloc(max_connections * sizeof(client_connection_t));
    if(!server->connections) {
        free(server);
        return NULL;
    }

    server->socket_fd = socket_fd;
    server->commands_fd = commands_fd;
    server->num_connections = 0;
    server->max_connections = max_connections;
    server->running = 0;

    return server;
}

int command_server_listen(command_server_t* server)
{
    if(!server || server->running) {
        return -1;
    }

    server->running = 1;
    if(pthread_create(&server->thread_id, NULL, server_thread, server) != 0) {
        server->running = 0;
        return -1;
    }

    return 0;
}

char* command_server_recv(command_server_t* server)
{
    if(!server) {
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes = read(server->commands_fd, buffer, BUFFER_SIZE - 1);

    if(bytes <= 0) {
        return NULL;
    }

    buffer[bytes] = '\0';
    char* result = strdup(buffer);
    if(!result) {
        return NULL;
    }

    return result;
}

int command_server_broadcast(command_server_t* server,
                                    const char* message)
{
    if(!server || !message) {
        return -1;
    }

    size_t message_len = strlen(message);
    int success = 0;

    // Send message to all connected clients
    for(size_t i = 0; i < server->num_connections; i++) {
        ssize_t bytes_sent =
            write(server->connections[i].fd, message, message_len);
        if(bytes_sent == (ssize_t)message_len) {
            success = 1;
        }
    }

    return success ? 0 : -1;
}

void command_server_destroy(command_server_t* server)
{
    if(!server) {
        return;
    }

    // Stop the server thread
    server->running = 0;
    pthread_join(server->thread_id, NULL);

    // Close all client connections
    for(size_t i = 0; i < server->num_connections; i++) {
        close(server->connections[i].fd);
    }

    // Clean up resources
    free(server->connections);
    free(server);
}