# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: sample.proto
# Protobuf Python Version: 5.28.2
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(
    _runtime_version.Domain.PUBLIC,
    5,
    28,
    2,
    '',
    'sample.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0csample.proto\"\xb3\x01\n\x06Sample\x12\x11\n\tmetric_id\x18\x01 \x01(\t\x12\x11\n\ttimestamp\x18\x02 \x01(\x02\x12\x11\n\x07int_val\x18\x03 \x01(\x05H\x00\x12\x12\n\x08long_val\x18\x04 \x01(\x03H\x00\x12\x13\n\tfloat_val\x18\x05 \x01(\x02H\x00\x12\x14\n\ndouble_val\x18\x06 \x01(\x01H\x00\x12\x12\n\x08\x62ool_val\x18\x07 \x01(\x08H\x00\x12\x14\n\nstring_val\x18\x08 \x01(\tH\x00\x42\x07\n\x05valueb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'sample_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_SAMPLE']._serialized_start=17
  _globals['_SAMPLE']._serialized_end=196
# @@protoc_insertion_point(module_scope)