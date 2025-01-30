#pragma once

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
    int socket_fd;   /**< Socket file descriptor */
    char* metric_id; /**< Metric ID */
} Requester;

/**
 * @brief Structure to hold arguments for making a requester.
 */
typedef struct {
    char* metric_id; /**< Metric ID */
    char* node;      /**< Node address */
    char* service;   /**< Service address */
} MakeRequesterArgs;

/**
 * @brief Creates a requester.
 *
 * @param args Arguments for making a requester.
 * @return Requester structure.
 */
Requester make_requester(MakeRequesterArgs args);

/**
 * @brief Structure to hold the result of an integer request.
 */
typedef struct {
    int err;   /**< Error code */
    int value; /**< Integer value */
} RequestIntResult;

/**
 * @brief Sends requests an integer value sample from the metrid id of reqr
 * from the server reqr.socket_fd is connected to.
 *
 * @param reqr Requester structure.
 * @return RequestIntResult structure containing error code and integer value.
 */
RequestIntResult request_int(Requester reqr);