#include "send_telemetry.h"
#include "sample.pb.h"
#include <pb_encode.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> // send()


int test_one(int socket_fd) {
    // test nanopb
    Sample mymessage = Sample_init_zero;
    mymessage.timestamp = 123.456;
    uint8_t buffer[128];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    pb_encode(&stream, Sample_fields, &mymessage);
    printf("Encoded message size: %d\n", (int) stream.bytes_written);
    for (int i = 0; i < stream.bytes_written; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("END\n");
    send(socket_fd, buffer, stream.bytes_written, 0);

    send_sample_int32(socket_fd, "temp", 0.0, 0);
    send_sample_int32(socket_fd, "hi", 0.0, 0);
    return 0;
}