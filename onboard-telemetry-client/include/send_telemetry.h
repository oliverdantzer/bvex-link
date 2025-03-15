#pragma once

/** @file send_telemetry.h
 *  @brief API to send telemetry data to the onboard server.
 *
 *  Example usage:
 *  @code
 *  #include "send_telemetry.h"
 *  #include <ctime>
 *  int main() {
 *      int socket_fd = connected_udp_socket(SAMPLE_SERVER_ADDR,
 *                                                 SAMPLE_SERVER_PORT);
 *      if (socket_fd < 0) {
 *          printf("Error connecting to server\n");
 *          return;
 *      }
 *      send_sample_int32(socket_fd, "altitude", time(NULL), 50392);
 *      send_sample_float(socket_fd, "yaw", time(NULL), 0.1452f);
 *      close(socket_fd);
 *      return 0;
 *  }
 *  @endcode
 */

#include <stdbool.h>
#include <stdint.h>

#define GETADDRINFO_ERROR 2
#define STRING_VALUE_MAX_SIZE 4096
#define METRIC_ID_MAX_SIZE 64
#define FILE_PATH_MAX_SIZE 128
#define EXTENSION_MAX_SIZE 16

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
int connected_udp_socket(char* node, char* service);

/**
 * @brief Sends an int32_t telemetry sample.
 *
 * @param socket_fd The socket file descriptor.
 * @param metric_id The identifier for the metric.
 * @param timestamp The timestamp of the sample.
 * @param value The int32_t value to send.
 */
void send_sample_int32(int socket_fd, char metric_id[METRIC_ID_MAX_SIZE], float timestamp,
                       int32_t value);

/**
 * @brief Sends an int64_t telemetry sample.
 *
 * @param socket_fd The socket file descriptor.
 * @param metric_id The identifier for the metric.
 * @param timestamp The timestamp of the sample.
 * @param value The int64_t value to send.
 */
void send_sample_int64(int socket_fd, char metric_id[METRIC_ID_MAX_SIZE], float timestamp,
                       int64_t value);

/**
 * @brief Sends a float telemetry sample.
 *
 * @param socket_fd The socket file descriptor.
 * @param metric_id The identifier for the metric.
 * @param timestamp The timestamp of the sample.
 * @param value The float value to send.
 */
void send_sample_float(int socket_fd, char metric_id[METRIC_ID_MAX_SIZE], float timestamp,
                       float value);

/**
 * @brief Sends a double telemetry sample.
 *
 * @param socket_fd The socket file descriptor.
 * @param metric_id The identifier for the metric.
 * @param timestamp The timestamp of the sample.
 * @param value The double value to send.
 */
void send_sample_double(int socket_fd, char metric_id[METRIC_ID_MAX_SIZE], float timestamp,
                        double value);

/**
 * @brief Sends a boolean telemetry sample.
 *
 * @param socket_fd The socket file descriptor.
 * @param metric_id The identifier for the metric.
 * @param timestamp The timestamp of the sample.
 * @param value The boolean value to send.
 */
void send_sample_bool(int socket_fd, char metric_id[METRIC_ID_MAX_SIZE],
                      float timestamp, bool value);

/**
 * @brief Sends a string telemetry sample.
 *
 * @note The string value must have a null terminator.
 *
 * @param socket_fd The socket file descriptor.
 * @param metric_id The identifier for the metric.
 * @param timestamp The timestamp of the sample.
 * @param value The string value to send.
 */
void send_sample_string(int socket_fd, char metric_id[METRIC_ID_MAX_SIZE],
                        float timestamp, char value[STRING_VALUE_MAX_SIZE]);

/**
 * @brief Sends a file telemetry sample.
 *
 * @param socket_fd The socket file descriptor.
 * @param metric_id The identifier for the metric.
 * @param timestamp The timestamp of the sample.
 * @param filepath The path to the file to send.
 * @param extension The file extension.
 */
void send_sample_file(int socket_fd, char metric_id[METRIC_ID_MAX_SIZE],
                      float timestamp, char filepath[FILE_PATH_MAX_SIZE],
                      char extension[EXTENSION_MAX_SIZE]);