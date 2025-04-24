#include "primitive.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <metric_id> <timestamp> <primitive-type>\n", argv[0]);
        return 1;
    }
    char *metric_id = argv[1];
    float timestamp = atof(argv[2]);
    char *sample;
    if (strcmp(argv[3], "int") == 0) {
        sample = encode_int_sample(metric_id, timestamp, 123);
    } else if (strcmp(argv[3], "float") == 0) {
        sample = encode_float_sample(metric_id, timestamp, 123.456);
    } else if (strcmp(argv[3], "bool") == 0) {
        sample = encode_bool_sample(metric_id, timestamp, true);
    } else {
        printf("Invalid primitive type: %s\n", argv[3]);
        return 1;
    }
    printf("%s", sample);
    free(sample);
    return 0;
}