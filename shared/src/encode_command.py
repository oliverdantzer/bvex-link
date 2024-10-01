import json
from typing import TypedDict

class RetransmitCommand(TypedDict):
    command: str
    sample_id: str
    seq_nums: list[int]

type Command = RetransmitCommand

def retransmit_segment_command(sample_id: str, seq_nums: list[int]) -> RetransmitCommand:
    return {
        "command": "retransmit_segment",
        "sample_id": sample_id,
        "seq_nums": seq_nums
    }


def encode_commands(commands: list[Command]) -> bytes:
    return json.dumps(commands).encode()