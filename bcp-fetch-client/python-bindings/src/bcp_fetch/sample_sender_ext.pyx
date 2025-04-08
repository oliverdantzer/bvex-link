'''
Actual implementation for wrapper class :class:`SampleSender` wrapping native `SampleSender` struct.
'''

__all__ = ['SampleSender']

from libc.errno cimport ENOMEM, EINVAL, ERANGE
from libc.string cimport strdup
from libc.stdlib cimport free
from cpython.unicode cimport PyUnicode_AsUTF8
from cython.operator import dereference
from typing import Generic, TypeVar
from bcp_fetch.c_sample_sender cimport (
    sample_sender_params_t,
    sample_sender_status_t,
    make_sample_sender_int32,
    make_sample_sender_float,
    make_sample_sender_string,
    destroy_sample_sender,
    sample_sender_t,
    sample_sender_int32_t,
    sample_sender_float_t,
    sample_sender_string_t,
    send_int32,
    send_float,
    send_string,
    SEND_SAMPLE_STATUS_OK,
    SEND_SAMPLE_STATUS_ENCODING_ERROR,
    SEND_SAMPLE_STATUS_SEND_ERROR,
    SEND_SAMPLE_STATUS_INVALID_PARAMETER,
    SEND_SAMPLE_STATUS_SENDER_NULL
)

T = TypeVar('T', int, float, str)
cdef class SampleSender(Generic[T]):
    def __init__(self, str metric_id, str node, str service):
        cdef sample_sender_params_t params
        cdef char* c_metric_id = strdup(PyUnicode_AsUTF8(metric_id))
        cdef char* c_node = strdup(PyUnicode_AsUTF8(node))
        cdef char* c_service = strdup(PyUnicode_AsUTF8(service))
        
        params.metric_id = c_metric_id
        params.node = c_node
        params.service = c_service

        cdef sample_sender_status_t* status = NULL

        if T is int:
            self._native = <sample_sender_t*>make_sample_sender_int32(params, status)
        elif T is float:
            self._native = <sample_sender_t*>make_sample_sender_float(params, status)
        elif T is str:
            self._native = <sample_sender_t*>make_sample_sender_string(params, status)
        else:
            raise ValueError("Invalid type")
        
        cdef sample_sender_status_t status_val = dereference(status)
        if status_val == SEND_SAMPLE_STATUS_OK:
            pass
        elif status_val == SEND_SAMPLE_STATUS_ENCODING_ERROR:
            raise ValueError("Failed to create sample sender, encoding error")
        elif status_val == SEND_SAMPLE_STATUS_SEND_ERROR:
            raise ValueError("Failed to create sample sender, send error")
        elif status_val == SEND_SAMPLE_STATUS_INVALID_PARAMETER:
            raise ValueError("Failed to create sample sender, invalid parameter")
        elif status_val == SEND_SAMPLE_STATUS_SENDER_NULL:
            raise ValueError("Failed to create sample sender, sender is null")

    def __dealloc__(self):
        destroy_sample_sender(self._native)

    def send_sample(self, float timestamp, T value):
        if T is int:
            assert isinstance(value, int)
            send_int32(
                <sample_sender_int32_t*>self._native,
                timestamp,
                value
            )
        elif T is float:
            assert isinstance(value, float)
            send_float(
                <sample_sender_float_t*>self._native,
                timestamp,
                value
            )
        elif T is str:
            assert isinstance(value, str)
            cdef char* c_value = strdup(PyUnicode_AsUTF8(value))
            send_string(
                <sample_sender_string_t*>self._native,
                timestamp,
                c_value
            )
            free(<void*>c_value)
        else:
            raise ValueError("Invalid type")
