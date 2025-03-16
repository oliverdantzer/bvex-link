#include "request.h"

int main() {
    Requester requester = make_requester("altitude", "localhost", "3000");
    if (requester.socket_fd < 0) {
        printf("Error creating requester\n");
        return;
    }
    RequestDoubleResult result = request_double(&requester);
    if (result.err) {
        printf("Request error\n");
        return;
    }
    printf("Response value: %f\n", result.value);
}