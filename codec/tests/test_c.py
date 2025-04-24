import os
c_test_executable_path = os.path.join(
    os.path.dirname(__file__), "..", "build", "bvex_codec_test")

def test_encode_int_sample():
    metric_id = "test_metric"
    timestamp = 1234567890
    value = 123
    sample = os.popen(f"{c_test_executable_path} {metric_id} {timestamp} int").read()
    assert sample == f'{{"metric_id":"{metric_id}","timestamp":{timestamp},"which_primitive":"int","value":{value}}}'

def test_encode_float_sample():
    metric_id = "test_metric"
    timestamp = 1234567890
    value = 123.456
    sample = os.popen(f"{c_test_executable_path} {metric_id} {timestamp} float").read()
    assert sample == f'{{"metric_id":"{metric_id}","timestamp":{timestamp},"which_primitive":"float","value":{value}}}'

def test_encode_bool_sample():
    metric_id = "test_metric"
    timestamp = 1234567890
    value = True
    sample = os.popen(f"{c_test_executable_path} {metric_id} {timestamp} bool").read()
    assert sample == f'{{"metric_id":"{metric_id}","timestamp":{timestamp},"which_primitive":"bool","value":{value}}}'

