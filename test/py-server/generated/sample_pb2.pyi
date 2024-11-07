from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class Sample(_message.Message):
    __slots__ = ("metric_id", "timestamp", "int_val", "long_val", "float_val", "double_val", "bool_val", "string_val")
    METRIC_ID_FIELD_NUMBER: _ClassVar[int]
    TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    INT_VAL_FIELD_NUMBER: _ClassVar[int]
    LONG_VAL_FIELD_NUMBER: _ClassVar[int]
    FLOAT_VAL_FIELD_NUMBER: _ClassVar[int]
    DOUBLE_VAL_FIELD_NUMBER: _ClassVar[int]
    BOOL_VAL_FIELD_NUMBER: _ClassVar[int]
    STRING_VAL_FIELD_NUMBER: _ClassVar[int]
    metric_id: str
    timestamp: float
    int_val: int
    long_val: int
    float_val: float
    double_val: float
    bool_val: bool
    string_val: str
    def __init__(self, metric_id: _Optional[str] = ..., timestamp: _Optional[float] = ..., int_val: _Optional[int] = ..., long_val: _Optional[int] = ..., float_val: _Optional[float] = ..., double_val: _Optional[float] = ..., bool_val: bool = ..., string_val: _Optional[str] = ...) -> None: ...
