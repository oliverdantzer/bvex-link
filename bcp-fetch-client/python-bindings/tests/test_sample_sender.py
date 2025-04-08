'''
Test cases for wrapper to native `bcp-fetch` library.
'''

import pytest

import bcp_fetch

def test_sample_sender():
    sender = bcp_fetch.SampleSender[str]("localhost", 5683, "test_topic")
    sender.send_string("Hello, World!")
