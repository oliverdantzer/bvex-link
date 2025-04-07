#include "sample.pb.h"
#include "sample_sender.h"
#include <stdbool.h>
#include <stdint.h>

bool validate_string(const char* str, size_t max_size);

struct sample_sender_s {
    int socket_fd;         // Socket file descriptor
    const char* metric_id; // Metric ID to send samples for
    Sample sample;
};

send_sample_status_t send_sample(sample_sender_t* sender, const Sample message);

sample_sender_t* make_sample_sender(sample_sender_params_t params,
                                    sample_sender_status_t* status);

void destroy_sample_sender(sample_sender_t* sender);