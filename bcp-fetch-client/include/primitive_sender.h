#pragma once

#include "sample_sender.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// Type definitions for primitive senders
typedef struct sample_sender_t sample_sender_primitive_t;
typedef struct sample_sender_t sample_sender_int32_t;
typedef struct sample_sender_t sample_sender_int64_t;
typedef struct sample_sender_t sample_sender_float_t;
typedef struct sample_sender_t sample_sender_double_t;
typedef struct sample_sender_t sample_sender_bool_t;
typedef struct sample_sender_t sample_sender_string_t;

// Function declarations for creating primitive senders
sample_sender_int32_t* make_sample_sender_int32(sample_sender_params_t params, sample_sender_status_t* status);
sample_sender_int64_t* make_sample_sender_int64(sample_sender_params_t params, sample_sender_status_t* status);
sample_sender_float_t* make_sample_sender_float(sample_sender_params_t params, sample_sender_status_t* status);
sample_sender_double_t* make_sample_sender_double(sample_sender_params_t params, sample_sender_status_t* status);
sample_sender_bool_t* make_sample_sender_bool(sample_sender_params_t params, sample_sender_status_t* status);
sample_sender_string_t* make_sample_sender_string(sample_sender_params_t params, sample_sender_status_t* status);

// Function declarations for sending primitive values
send_sample_status_t send_int32(const sample_sender_int32_t* sender, float timestamp, int32_t value);
send_sample_status_t send_int64(const sample_sender_int64_t* sender, float timestamp, int64_t value);
send_sample_status_t send_float(const sample_sender_float_t* sender, float timestamp, float value);
send_sample_status_t send_double(const sample_sender_double_t* sender, float timestamp, double value);
send_sample_status_t send_bool(const sample_sender_bool_t* sender, float timestamp, bool value);
send_sample_status_t send_string(const sample_sender_string_t* sender, float timestamp, const char* value);

#ifdef __cplusplus
}
#endif 