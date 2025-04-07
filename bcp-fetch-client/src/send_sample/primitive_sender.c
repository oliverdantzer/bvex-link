#include "primitive.pb.h"
#include "sample.pb.h"
#include "sample_sender.h"
#include "sample_sender_utils.h"

typedef sample_sender_t sample_sender_primitive_t;
static sample_sender_primitive_t* make_sample_sender_primitive(
    sample_sender_params_t params, pb_size_t primitive_val_tag,
    sample_sender_status_t* status)
{
    sample_sender_t* sender = make_sample_sender(params, status);
    if(sender == NULL) {
        return NULL;
    }
    sender->sample = (Sample)Sample_init_zero;
    strlcpy(sender->sample.metric_id, params.metric_id,
            member_size(Sample, metric_id));
    sender->sample.timestamp = 0.0f;
    sender->sample.which_data = Sample_primitive_tag;
    sender->sample.data.primitive =
        (primitive_Primitive)primitive_Primitive_init_zero;
    sender->sample.data.primitive.which_value = primitive_val_tag;
    *status = SAMPLE_SENDER_STATUS_OK;
    return sender;
}

static send_sample_status_t send_primitive(sample_sender_primitive_t* sender,
                                           float timestamp,
                                           primitive_Primitive value)
{
    if(sender->sample.data.primitive.which_value != value.which_value) {
        return SEND_SAMPLE_STATUS_INVALID_PARAMETER;
    }
    sender->sample.timestamp = timestamp;
    sender->sample.data.primitive = value;
    return send_sample(sender, sender->sample);
}

sample_sender_int32_t* make_sample_sender_int32(sample_sender_params_t params,
                                                sample_sender_status_t* status)
{
    return make_sample_sender_primitive(params, primitive_Primitive_int_val_tag,
                                        status);
}

send_sample_status_t send_int32(sample_sender_int32_t* sender, float timestamp,
                                int32_t value)
{
    primitive_Primitive primitive = primitive_Primitive_init_zero;
    primitive.which_value = primitive_Primitive_int_val_tag;
    primitive.value.int_val = value;
    return send_primitive(sender, timestamp, primitive);
}

sample_sender_float_t* make_sample_sender_float(sample_sender_params_t params,
                                                sample_sender_status_t* status)
{
    return make_sample_sender_primitive(
        params, primitive_Primitive_float_val_tag, status);
}

send_sample_status_t send_float(sample_sender_float_t* sender, float timestamp,
                                float value)
{
    primitive_Primitive primitive = primitive_Primitive_init_zero;
    primitive.which_value = primitive_Primitive_float_val_tag;
    primitive.value.float_val = value;
    return send_primitive(sender, timestamp, primitive);
}

sample_sender_string_t* make_sample_sender_string(
    sample_sender_params_t params, sample_sender_status_t* status)
{
    return make_sample_sender_primitive(
        params, primitive_Primitive_string_val_tag, status);
}

send_sample_status_t send_string(sample_sender_string_t* sender,
                                 float timestamp, const char* value)
{
    primitive_Primitive primitive = primitive_Primitive_init_zero;
    primitive.which_value = primitive_Primitive_string_val_tag;
    strlcpy(primitive.value.string_val, value,
            sizeof(primitive.value.string_val));
    return send_primitive(sender, timestamp, primitive);
}