#include "file_sender.h"
#include "sample_sender.h"
#include "generated/nanopb/sample.pb.h"

sample_sender_file_t* make_sample_sender_file(sample_sender_params_t params, sample_sender_status_t* status)
{
    sample_sender_t* sender = make_sample_sender(params, status);
    if (sender == NULL) {
        return NULL;
    }
    sender->sample = sample_file(params.metric_id, 0.0f, File_init_zero);
    return sender;
}

sample_status_t send_file(const sample_sender_file_t* sender, float timestamp, const char* filepath, const char* extension)
{
    if (!validate_string(filepath, sizeof(File_init_zero.filepath))) {
        return SEND_SAMPLE_STATUS_INVALID_PARAMETER;
    }
    if (!validate_string(extension, sizeof(File_init_zero.extension))) {
        return SEND_SAMPLE_STATUS_INVALID_PARAMETER;
    }
    sender->sample.timestamp = timestamp;
    sender->sample.data.file = File_init_zero;
    strlcpy(sender->sample.data.file.filepath, filepath, sizeof(sender->sample.data.file.filepath));
    strlcpy(sender->sample.data.file.extension, extension, sizeof(sender->sample.data.file.extension));
    return send_sample(sender->socket_fd, sender->sample);
}