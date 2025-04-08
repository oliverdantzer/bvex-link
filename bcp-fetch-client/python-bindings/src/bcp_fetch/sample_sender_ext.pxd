'''
Cython definitions for wrapper class :class:`SampleSender` wrapping native `Foo` struct.
'''

__all__ = ['SampleSender']

from typing import Generic, TypeVar
from bcp_fetch cimport c_sample_sender

T = TypeVar('T', int, float, str)

cdef class SampleSender(Generic[T]):
    '''
    Wrapper class keeping track of native `SampleSender` struct.
    '''

    #: Native struct keeping track of actual information.
    cdef c_sample_sender.sample_sender_t* _native