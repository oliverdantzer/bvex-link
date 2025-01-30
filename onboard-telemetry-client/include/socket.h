#pragma once

#define GETADDRINFO_ERROR 2

/**
 * @brief Makes UDP socket, connects to node:service, and returns socket file
 * descriptor.
 *
 * This function creates a UDP socket, connects it to the specified node and
 * service, and returns the socket file descriptor. The socket is not bound to a
 * specific port, allowing the OS to choose. The socket's endpoint is set to the
 * specified node and service using connect(), allowing you to send data to the
 * same address with send().
 *
 * @param node IP address or hostname.
 * @param service Port number or service name (e.g., "1234" or "http").
 * @return Socket file descriptor on success, or -1 on error.
 *
 * @note To close the socket, use close(fd), where fd is the socket's file
 * descriptor.
 */
int make_connected_send_socket(char* node, char* service);
