#pragma once

#include <stdbool.h>
#include <stdint.h>

void send_sample_int32(int socket_fd, char* metric_id, float timestamp,
                       int32_t value);
void send_sample_int64(int socket_fd, char* metric_id, float timestamp,
                       int64_t value);
void send_sample_float(int socket_fd, char* metric_id, float timestamp,
                       float value);
void send_sample_double(int socket_fd, char* metric_id, float timestamp,
                        double value);
void send_sample_bool(int socket_fd, char* metric_id, float timestamp,
                      bool value);
void send_sample_string(int socket_fd, char* metric_id, float timestamp,
                        char* value);
void send_sample_bytes(int socket_fd, char* metric_id, float timestamp,
                       uint8_t* value);
