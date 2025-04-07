#include "sample.pb.h"
#include "sample_sender.h"
#include "sample_sender_utils.h"

// Define the actual struct that was declared as opaque in the header
struct sample_sender_file_s {
    sample_sender_t base;
};

sample_sender_file_t* make_sample_sender_file(sample_sender_params_t params,
                                              sample_sender_status_t* status)
{
    sample_sender_t* base = make_sample_sender(params, status);
    if(base == NULL) {
        return NULL;
    }
    base->sample = (Sample)Sample_init_zero;
    strlcpy(base->sample.metric_id, params.metric_id,
            member_size(Sample, metric_id));
    base->sample.timestamp = 0.0f;
    base->sample.which_data = Sample_file_tag;
    base->sample.data.file = (File)File_init_zero;

    sample_sender_file_t* sender = (sample_sender_file_t*)base;
    return sender;
}

send_sample_status_t send_file(sample_sender_file_t* sender, float timestamp,
                               const char* filepath, const char* extension)
{
    if(!validate_string(filepath, sizeof((File)File_init_zero.filepath))) {
        return SEND_SAMPLE_STATUS_INVALID_PARAMETER;
    }
    if(!validate_string(extension, sizeof((File)File_init_zero.extension))) {
        return SEND_SAMPLE_STATUS_INVALID_PARAMETER;
    }
    sender->base.sample.timestamp = timestamp;
    sender->base.sample.data.file = (File)File_init_zero;
    strlcpy(sender->base.sample.data.file.filepath, filepath,
            sizeof(sender->base.sample.data.file.filepath));
    strlcpy(sender->base.sample.data.file.extension, extension,
            sizeof(sender->base.sample.data.file.extension));
    return send_sample(&sender->base, sender->base.sample);
}