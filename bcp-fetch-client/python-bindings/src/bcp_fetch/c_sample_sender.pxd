'''
Cython definitions matching `sample_sender.h` header.
'''

from libc.stdint cimport int32_t
from libc.stddef cimport size_t

cdef extern from 'sample_sender.h':
    ctypedef enum send_sample_status_t:
        SEND_SAMPLE_STATUS_OK
        SEND_SAMPLE_STATUS_ENCODING_ERROR
        SEND_SAMPLE_STATUS_SEND_ERROR
        SEND_SAMPLE_STATUS_INVALID_PARAMETER
        SEND_SAMPLE_STATUS_SENDER_NULL

    ctypedef struct sample_sender_params_t:
        const char* metric_id
        const char* node
        const char* service

    ctypedef enum sample_sender_status_t:
        SAMPLE_SENDER_STATUS_OK
        SAMPLE_SENDER_STATUS_INVALID_PARAMETER
        SAMPLE_SENDER_STATUS_SOCKET_CREATION_FAILED

    ctypedef struct sample_sender_s:
        pass
    ctypedef sample_sender_s sample_sender_t

    ctypedef struct sample_sender_int32_s:
        pass
    ctypedef sample_sender_int32_s sample_sender_int32_t

    ctypedef struct sample_sender_float_s:
        pass
    ctypedef sample_sender_float_s sample_sender_float_t

    ctypedef struct sample_sender_string_s:
        pass
    ctypedef sample_sender_string_s sample_sender_string_t

    ctypedef struct sample_sender_file_s:
        pass
    ctypedef sample_sender_file_s sample_sender_file_t

    # Constants
    extern const size_t METRIC_ID_MAX_SIZE
    extern const size_t PRIMITIVE_STRING_VALUE_MAX_SIZE
    extern const size_t FILE_PATH_MAX_SIZE
    extern const size_t EXTENSION_MAX_SIZE

    # Function declarations for creating primitive senders
    sample_sender_int32_t* make_sample_sender_int32(sample_sender_params_t params,
                                                   sample_sender_status_t* status)
    sample_sender_float_t* make_sample_sender_float(sample_sender_params_t params,
                                                   sample_sender_status_t* status)
    sample_sender_string_t* make_sample_sender_string(sample_sender_params_t params,
                                                     sample_sender_status_t* status)
    sample_sender_file_t* make_sample_sender_file(sample_sender_params_t params,
                                                 sample_sender_status_t* status)

    # Function declarations for sending primitive values
    send_sample_status_t send_int32(sample_sender_int32_t* sender, float timestamp,
                                   int32_t value)
    send_sample_status_t send_float(sample_sender_float_t* sender, float timestamp,
                                   float value)
    send_sample_status_t send_string(sample_sender_string_t* sender,
                                    float timestamp, const char* value)
    send_sample_status_t send_file(sample_sender_file_t* sender, float timestamp,
                                  const char* filepath, const char* extension)

    void destroy_sample_sender(sample_sender_t* sender)