#pragma once

#include "sample_sender.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// Type declarations for primitive senders
typedef sample_sender_t sample_sender_primitive_t;
typedef sample_sender_t sample_sender_int32_t;
typedef sample_sender_t sample_sender_float_t;
typedef sample_sender_t sample_sender_string_t;

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

#ifdef __cplusplus
}
#endif