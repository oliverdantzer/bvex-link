// int main(int argc, char** argv)
// {
//     int redis_port;
//     if(argc != 2) {
//             printf("Usage: %s <port>\n", argv[0]);
//             return 1;
//     } else {
//         char* endptr;
//         redis_port = strtol(argv[1], &endptr, 10);

//         if(*endptr != '\0' || redis_port <= 0 || redis_port > 65535) {
//             printf("Invalid port number arg\n");
//             return 1;
//         }
//     }
//     const char* localhost_ip = "127.0.0.1";

//     testSetSample(c);

//     while(1) {
//         // Get the current time
//         time_t t;
//         time(&t);

//         double temp =
//             generate_sinusoid(20.0, 1.0 / 60.0, 0.0, (double)t) + 20.0;
//         double altitude =
//             generate_sinusoid(1000.0, 1.0 / 120.0, 0.0, (double)t) + 10000.0;
//         double roll = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);
//         double pitch = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);
//         double yaw = generate_sinusoid(1.0, 1.0 / 60.0, 0.0, (double)t);

//         setSample(c, "temp", temp);
//         setSample(c, "altitude", altitude);
//         setSample(c, "roll", roll);
//         setSample(c, "pitch", pitch);
//         setSample(c, "yaw", yaw);

//         // Sleep 1 ms to mimic high sample rate
//         sleep(0.001);
//     }

//     redisFree(c);
//     return 0;
// }