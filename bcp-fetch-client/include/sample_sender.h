#pragma once

/** @file sample_sender.h
 *  @brief API to send sample data to the onboard server using a sample_sender
 * object.
 *
 *  Example usage:
 *  @code
 *  #include "sample_sender.h"
 *  #include <time.h>
 *
 *  int main() {
 *      SampleSender sender = make_sample_sender("altitude", "localhost",
 * "3000"); if (sender.socket_fd < 0) { printf("Error creating sender\n");
 *          return 1;
 *      }
 *      float current_time = time(NULL);
 *      send_int32(&sender, current_time, 50392);
 *      return 0;
 *  }
 *  @endcode
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "../src/generated/nanopb/sample.pb.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    SEND_SAMPLE_STATUS_OK = 0,
    SEND_SAMPLE_STATUS_ENCODING_ERROR,
    SEND_SAMPLE_STATUS_SEND_ERROR,
    SEND_SAMPLE_STATUS_INVALID_PARAMETER,
    SEND_SAMPLE_STATUS_SENDER_NULL
} send_sample_status_t;

typedef struct {
    const char* metric_id;
    const char* node;
    const char* service;
} sample_sender_params_t;

typedef enum {
    SAMPLE_SENDER_STATUS_OK = 0,
    SAMPLE_SENDER_STATUS_INVALID_PARAMETER,
    SAMPLE_SENDER_STATUS_SOCKET_CREATION_FAILED,
} sample_sender_status_t;

#define member_size(type, member) (sizeof(((type*)0)->member))

#define STRING_VALUE_MAX_SIZE 4096
#define METRIC_ID_MAX_SIZE 64
#define FILE_PATH_MAX_SIZE 128
#define EXTENSION_MAX_SIZE 16

bool validate_string(const char* str, size_t max_size);

typedef struct {
    int socket_fd;         // Socket file descriptor
    const char* metric_id; // Metric ID to send samples for
    Sample sample;
} sample_sender_t;

send_sample_status_t send_sample(sample_sender_t* sender, const Sample message);

sample_sender_t* make_sample_sender(sample_sender_params_t params,
                                    sample_sender_status_t* status);

void destroy_sample_sender(sample_sender_t* sender);

#ifdef __cplusplus
}
#endif