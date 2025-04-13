"""
BCP Redis Client - A client for handling sample data in Redis with type safety.

This package provides functionality for encoding, decoding, and managing sample data
in Redis with support for primitive types (int, float, str, bytes, bool).
"""

from .sample import set_sample_primitive, SampleSubscriber

__all__ = [
    "set_sample_primitive",
    "SampleSubscriber",
]

__version__ = "0.1.0"
