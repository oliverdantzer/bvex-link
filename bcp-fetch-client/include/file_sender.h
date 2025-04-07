#pragma once

#include "sample_sender.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// Type definition for file sender
typedef sample_sender_t sample_sender_file_t;

// Function to create a file sender
sample_sender_file_t* make_sample_sender_file(sample_sender_params_t params,
                                              sample_sender_status_t* status);

// Function to send a file
send_sample_status_t send_file(sample_sender_file_t* sender, float timestamp,
                               const char* filepath, const char* extension);

#ifdef __cplusplus
}
#endif