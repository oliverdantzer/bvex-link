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

#include <stdbool.h>
#include <stddef.h>
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

extern const size_t METRIC_ID_MAX_SIZE;
extern const size_t PRIMITIVE_STRING_VALUE_MAX_SIZE;
extern const size_t FILE_PATH_MAX_SIZE;
extern const size_t EXTENSION_MAX_SIZE;

struct sample_sender_s;
typedef struct sample_sender_s sample_sender_t;

struct sample_sender_int32_s;
typedef struct sample_sender_int32_s sample_sender_int32_t;

struct sample_sender_float_s;
typedef struct sample_sender_float_s sample_sender_float_t;

struct sample_sender_string_s;
typedef struct sample_sender_string_s sample_sender_string_t;

struct sample_sender_file_s;
typedef struct sample_sender_file_s sample_sender_file_t;

// Function declarations for creating primitive senders
sample_sender_int32_t* make_sample_sender_int32(sample_sender_params_t params,
                                                sample_sender_status_t* status);
sample_sender_float_t* make_sample_sender_float(sample_sender_params_t params,
                                                sample_sender_status_t* status);
sample_sender_string_t* make_sample_sender_string(
    sample_sender_params_t params, sample_sender_status_t* status);

// Function declarations for sending primitive values
send_sample_status_t send_int32(sample_sender_int32_t* sender, float timestamp,
                                int32_t value);
send_sample_status_t send_float(sample_sender_float_t* sender, float timestamp,
                                float value);
send_sample_status_t send_string(sample_sender_string_t* sender,
                                 float timestamp, const char* value);

sample_sender_file_t* make_sample_sender_file(sample_sender_params_t params,
                                              sample_sender_status_t* status);

send_sample_status_t send_file(sample_sender_file_t* sender, float timestamp,
                               const char* filepath, const char* extension);

void destroy_sample_sender(sample_sender_t* sender);

#ifdef __cplusplus
}
#endif