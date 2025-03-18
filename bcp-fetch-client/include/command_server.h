#pragma once

#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>

// Structure to hold a client connection
typedef struct {
    int fd;                  // Client socket file descriptor
    struct sockaddr_in addr; // Client address information
} client_connection_t;

// Structure to hold the command server state
typedef struct {
    int socket_fd;                    // Server socket file descriptor
    int commands_fd;                  // File descriptor for commands output
    client_connection_t* connections; // Array of client connections
    size_t num_connections;           // Number of active connections
    size_t max_connections;           // Maximum number of connections allowed
    pthread_t thread_id;              // Server thread ID
    int running;                      // Flag to control server thread
} command_server_t;

/**
 * @brief Creates and initializes a new command server
 *
 * @param socket_fd The server socket file descriptor
 * @param commands_fd The file descriptor to write commands to
 * @param max_connections Maximum number of simultaneous connections
 * @return Pointer to the created server, or NULL on error
 */
command_server_t* command_server_create(int socket_fd, int commands_fd,
                                        size_t max_connections);

/**
 * @brief Starts the command server listening thread
 *
 * @param server Pointer to the command server
 * @return 0 on success, -1 on error
 */
int command_server_listen(command_server_t* server);

/**
 * @brief Reads oldest unread command
 *
 * @param server Pointer to the command server
 * @return char* Dynamically allocated string containing the command, or NULL on
 * error. The caller is responsible for freeing the returned string.
 */
char* command_server_recv(command_server_t* server);

/**
 * @brief Cleans up and destroys the command server
 *
 * @param server Pointer to the command server
 */
void command_server_destroy(command_server_t* server);