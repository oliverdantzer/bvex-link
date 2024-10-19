Onboard software: ./onboard-link/
Command software ./command/

Max sample rate = 1Hz
32-bit unix time (breaks in 2038)

For each sample type:
Sol defines sample rate

Two telemetry sample types:
File and json

Telemetry header:

Bord has PACKET_SIZE variable

Bord determines segment size and number for sample from PACKET_SIZE

Bord downlink file packet schema:
SAMPLE-TIME(seconds since last epoch) -- This will act as unique id for file
SEGMENT NUMBER(int)
TOTAL NUM OF SEGMENTS(int)
SEGMENT DATA

each metric has dict of loaded time :

bord: `cd bord && source ./win-venv.sh && python src/main.py --dev`
