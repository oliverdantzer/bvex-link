#include <stdio.h>
#include <math.h>
#include <hiredis/hiredis.h>
#include <time.h>
#include <unistd.h> 
#include <stdlib.h>

// Function to generate a sine wave value
double generate_sinusoid(double amplitude, double freq, double shift, double x) {
    return amplitude * sin(freq * x + shift);
}

// Function to set a sample in Redis
void setSample(redisContext *c, const char* field_name, double value) {
    char command[256];
    snprintf(command, sizeof(command), "HSET samples %s %f", field_name, value);
    redisReply *reply = redisCommand(c, command);
    if (reply == NULL) {
        printf("Error: %s\n", c->errstr);
        redisFree(c);
        return;
    }
    if (reply->type == REDIS_REPLY_ERROR) {
        printf("Error: %s\n", reply->str);
    }

    freeReplyObject(reply);
}

// runs on order of 10^-4 (hundreds of microseconds)
void testSetSample(redisContext *c) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    setSample(c, "temp", 1.0);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("setSample took %f seconds to execute\n", cpu_time_used);
}

int main(int argc, char **argv) {
    int redis_port;
    if (argc != 2) {
        if (argc == 1) {
            // No arguments provided, set default port to 6379
            redis_port = 6379;
        } else {
            printf("Usage: %s <redis_port>\n", argv[0]);
            return 1;
        }
    } else {
        char *endptr;
        redis_port = strtol(argv[1], &endptr, 10);

        if (*endptr != '\0' || redis_port <= 0 || redis_port > 65535) {
            printf("Invalid port number arg\n");
            return 1;
        }
    }
    const char *localhost_ip = "127.0.0.1";
    redisContext *c = redisConnect(localhost_ip, redis_port);
    if (c == NULL || c->err) {
        if (c) {
            printf("Error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Can't allocate redis context\n");
        }
        return 1;
    }

    testSetSample(c);

    while (1) {
        // Get the current time
        time_t t;
        time(&t);
        
        setSample(c, "temp", generate_sinusoid(20.0, 1.0/60.0, 0.0, (double)t) + 20.0);
        setSample(c, "altitude", generate_sinusoid(1000.0, 1.0/120.0, 0.0, (double)t) + 10000.0);
        setSample(c, "roll", generate_sinusoid(1.0, 1.0/60.0, 0.0, (double)t));
        setSample(c, "pitch", generate_sinusoid(1.0, 1.0/60.0, 0.0, (double)t));
        setSample(c, "yaw", generate_sinusoid(1.0, 1.0/60.0, 0.0, (double)t));

        // Sleep 1 ms to mimic high sample rate
        sleep(0.001);
    }

    redisFree(c);
    return 0;
}