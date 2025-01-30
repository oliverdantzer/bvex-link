#pragma once

/** @file
 *  @brief API to request telemetry data from the onboard server.
 *
 *  Example usage:
 *  @example example_usage.c
 */


#include "generated/nanopb/request.pb.h"
#include "generated/nanopb/response.pb.h"

#define ERR_SOCKET_CREATION -1
#define ERR_ENCODING_FAILED -2
#define ERR_SEND_FAILED -3
#define ERR_RECV_FAILED -4
#define ERR_DECODING_FAILED -5
#define ERR_INVALID_RESPONSE_TYPE -6

/**
 * @brief Structure to hold requester information.
 */
typedef struct {
    int socket_fd;   // Socket file descriptor
    char* metric_id; // Metric ID
} Requester;

/**
 * @brief Structure to hold arguments for making a requester.
 */
typedef struct {
    char* metric_id; // Metric ID
    char* node;      // Node address
    char* service;   // Service address
} MakeRequesterArgs;

/**
 * @brief Creates a requester. Used by functions like request_int to
 * specify endpoint and metric ID to request a value from. Threading multiple
 * requests to the same Requester is not recommended.
 *
 * @param metric_id The metric ID to request.
 * @param node The node to connect to.
 * @param service The service to connect to.
 * @return Requester structure.
 *
 * This requester object is used instead of requesting a metric directly from a
 * socket file descriptor to make it difficult to request different metric ids
 * from the same socket.
 *
 * Example usage:
 * ```
 * Requester altitudeRequester = make_requester(
 *    "altitude", "localhost", "3000");
 * if (altitudeRequester.socket_fd < 0) {
 *    // handle error
 * } else {
 *    // use altitudeRequester
 * }
 * ```
 */
Requester make_requester(char* metric_id, char* node, char* service);

/**
 * @brief Structure to hold the result of an integer request.
 */
typedef struct {
    int err;   // Error code
    int value; // Integer value
} RequestIntResult;

/**
 * @brief Sends requests an integer value sample from the metrid id of reqr
 * from the server reqr.socket_fd is connected to.
 *
 * @param reqr Requester structure.
 * @return RequestIntResult structure containing error code and integer value.
 *
 * Example usage:
 * ```
 * RequestIntResult result = request_int(&requester)
 * if (result.err) {
 *    // handle error
 * } else {
 *    // use result.value
 * }
 * ```
 */
RequestIntResult request_int(Requester* reqr);

/**
 * @brief Structure to hold the result of a string request.
 */
typedef struct {
    int err;     // Error code
    char* value; // String value
} RequestStringResult;

/**
 * @brief Sends a request for a string value sample from the metric id of reqr
 * from the server reqr.socket_fd is connected to.
 *
 * @param reqr Requester structure.
 * @return RequestStringResult structure containing error code and string value.
 *
 * Example usage:
 * ```
 * RequestStringResult result = request_string(&requester)
 * if (result.err) {
 *    // handle error
 * } else {
 *    // use result.value
 * }
 * ```
 */
RequestStringResult request_string(Requester* reqr);

/**
 * @brief Structure to hold the result of a double request.
 */
typedef struct {
    int err;      // Error code
    double value; // Double value
} RequestDoubleResult;

/**
 * @brief Sends a request for a double value sample from the metric id of reqr
 * from the server reqr.socket_fd is connected to. This function is blocking
 * and will wait for the send operation to complete and for the response to be
 * received.
 *
 * @param reqr Requester structure.
 * @return RequestDoubleResult structure containing error code and double value.
 *
 * Example usage:
 * ```
 * RequestDoubleResult result = request_double(&requester)
 * if (result.err) {
 *    // handle error
 * } else {
 *    // use result.value
 * }
 * ```
 */
RequestDoubleResult request_double(Requester* reqr);