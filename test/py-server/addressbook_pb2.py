# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: addressbook.proto
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
    'addressbook.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x11\x61\x64\x64ressbook.proto\x12\x08tutorial\"\xa3\x02\n\x06Person\x12\x0c\n\x04name\x18\x01 \x01(\t\x12\n\n\x02id\x18\x02 \x01(\x05\x12\r\n\x05\x65mail\x18\x03 \x01(\t\x12,\n\x06phones\x18\x04 \x03(\x0b\x32\x1c.tutorial.Person.PhoneNumber\x1aX\n\x0bPhoneNumber\x12\x0e\n\x06number\x18\x01 \x01(\t\x12\x39\n\x04type\x18\x02 \x01(\x0e\x32\x1a.tutorial.Person.PhoneType:\x0fPHONE_TYPE_HOME\"h\n\tPhoneType\x12\x1a\n\x16PHONE_TYPE_UNSPECIFIED\x10\x00\x12\x15\n\x11PHONE_TYPE_MOBILE\x10\x01\x12\x13\n\x0fPHONE_TYPE_HOME\x10\x02\x12\x13\n\x0fPHONE_TYPE_WORK\x10\x03\"/\n\x0b\x41\x64\x64ressBook\x12 \n\x06people\x18\x01 \x03(\x0b\x32\x10.tutorial.Person')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'addressbook_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_PERSON']._serialized_start=32
  _globals['_PERSON']._serialized_end=323
  _globals['_PERSON_PHONENUMBER']._serialized_start=129
  _globals['_PERSON_PHONENUMBER']._serialized_end=217
  _globals['_PERSON_PHONETYPE']._serialized_start=219
  _globals['_PERSON_PHONETYPE']._serialized_end=323
  _globals['_ADDRESSBOOK']._serialized_start=325
  _globals['_ADDRESSBOOK']._serialized_end=372
# @@protoc_insertion_point(module_scope)
