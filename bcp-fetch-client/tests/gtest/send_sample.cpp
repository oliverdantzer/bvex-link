#include "../common/decode_sample.h"
#include <arpa/inet.h>
#include <connected_udp_socket.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include "primitive_sender.h"
#include "file_sender.h"
#include "sample_sender.h"
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Helper function to create a test UDP server
static int create_test_server(const char* port)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(port));

    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) ==
       -1) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

// Helper function to receive data from the server
static ssize_t receive_data(int server_fd, char* buffer, size_t buffer_size)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    return recvfrom(server_fd, buffer, buffer_size, 0,
                    (struct sockaddr*)&client_addr, &addr_len);
}

class SendSampleTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Create test server
        server_fd = create_test_server("8080");
        ASSERT_NE(server_fd, -1);

        // Set up common sender params
        sender_params = {.metric_id = nullptr, // Will be set per test
                         .node = "localhost",
                         .service = "8080"};
    }

    void TearDown() override
    {
        if(server_fd != -1) {
            close(server_fd);
        }
    }

    int server_fd = -1;
    sample_sender_params_t sender_params;
};

TEST_F(SendSampleTest, SendInt32Sample)
{
    const char* metric_id = "test_int32";
    float timestamp = 1234.5f;
    int32_t value = 42;

    sender_params.metric_id = metric_id;
    sample_sender_status_t status;
    sample_sender_int32_t* sender =
        make_sample_sender_int32(sender_params, &status);
    ASSERT_EQ(status, SAMPLE_SENDER_STATUS_OK);
    ASSERT_NE(sender, nullptr);

    send_sample_status_t send_status = send_int32(sender, timestamp, value);
    EXPECT_EQ(send_status, SEND_SAMPLE_STATUS_OK);

    char buffer[1024];
    ssize_t received = receive_data(server_fd, buffer, sizeof(buffer));
    EXPECT_GT(received, 0);

    Sample* decoded_sample = decode_sample((uint8_t*)buffer, received);
    ASSERT_NE(decoded_sample, nullptr);
    EXPECT_EQ(decoded_sample->which_data, Sample_primitive_tag);
    EXPECT_EQ(decoded_sample->data.primitive.which_value,
              primitive_Primitive_int_val_tag);
    EXPECT_EQ(decoded_sample->data.primitive.value.int_val, value);
    EXPECT_EQ(decoded_sample->timestamp, timestamp);
    EXPECT_STREQ(decoded_sample->metric_id, metric_id);
    free(decoded_sample);
    destroy_sample_sender(sender);
}

TEST_F(SendSampleTest, SendFloatSample)
{
    const char* metric_id = "test_float";
    float timestamp = 1234.5f;
    float value = 3.14159f;

    sender_params.metric_id = metric_id;
    sample_sender_status_t status;
    sample_sender_float_t* sender =
        make_sample_sender_float(sender_params, &status);
    ASSERT_EQ(status, SAMPLE_SENDER_STATUS_OK);
    ASSERT_NE(sender, nullptr);

    send_sample_status_t send_status = send_float(sender, timestamp, value);
    EXPECT_EQ(send_status, SEND_SAMPLE_STATUS_OK);

    char buffer[1024];
    ssize_t received = receive_data(server_fd, buffer, sizeof(buffer));
    EXPECT_GT(received, 0);

    Sample* decoded_sample = decode_sample((uint8_t*)buffer, received);
    ASSERT_NE(decoded_sample, nullptr);
    EXPECT_EQ(decoded_sample->which_data, Sample_primitive_tag);
    EXPECT_EQ(decoded_sample->data.primitive.which_value,
              primitive_Primitive_float_val_tag);
    EXPECT_FLOAT_EQ(decoded_sample->data.primitive.value.float_val, value);
    EXPECT_EQ(decoded_sample->timestamp, timestamp);
    EXPECT_STREQ(decoded_sample->metric_id, metric_id);
    free(decoded_sample);
    destroy_sample_sender(sender);
}

TEST_F(SendSampleTest, SendStringSample)
{
    const char* metric_id = "test_string";
    float timestamp = 1234.5f;
    const char* value = "Hello, World!";

    sender_params.metric_id = metric_id;
    sample_sender_status_t status;
    sample_sender_string_t* sender =
        make_sample_sender_string(sender_params, &status);
    ASSERT_EQ(status, SAMPLE_SENDER_STATUS_OK);
    ASSERT_NE(sender, nullptr);

    send_sample_status_t send_status = send_string(sender, timestamp, value);
    EXPECT_EQ(send_status, SEND_SAMPLE_STATUS_OK);

    char buffer[1024];
    ssize_t received = receive_data(server_fd, buffer, sizeof(buffer));
    EXPECT_GT(received, 0);

    Sample* decoded_sample = decode_sample((uint8_t*)buffer, received);
    ASSERT_NE(decoded_sample, nullptr);
    EXPECT_EQ(decoded_sample->which_data, Sample_primitive_tag);
    EXPECT_EQ(decoded_sample->data.primitive.which_value,
              primitive_Primitive_string_val_tag);
    EXPECT_STREQ(decoded_sample->data.primitive.value.string_val, value);
    EXPECT_EQ(decoded_sample->timestamp, timestamp);
    EXPECT_STREQ(decoded_sample->metric_id, metric_id);
    free(decoded_sample);
    destroy_sample_sender(sender);
}

TEST_F(SendSampleTest, SendFileSample)
{
    const char* metric_id = "test_file";
    float timestamp = 1234.5f;
    const char* filepath = "test.txt";
    const char* extension = "txt";

    sender_params.metric_id = metric_id;
    sample_sender_status_t status;
    sample_sender_file_t* sender =
        make_sample_sender_file(sender_params, &status);
    ASSERT_EQ(status, SAMPLE_SENDER_STATUS_OK);
    ASSERT_NE(sender, nullptr);

    send_sample_status_t send_status =
        send_file(sender, timestamp, filepath, extension);
    EXPECT_EQ(send_status, SEND_SAMPLE_STATUS_OK);

    char buffer[1024];
    ssize_t received = receive_data(server_fd, buffer, sizeof(buffer));
    EXPECT_GT(received, 0);

    Sample* decoded_sample = decode_sample((uint8_t*)buffer, received);
    ASSERT_NE(decoded_sample, nullptr);
    EXPECT_EQ(decoded_sample->which_data, Sample_file_tag);
    EXPECT_STREQ(decoded_sample->data.file.filepath, filepath);
    EXPECT_STREQ(decoded_sample->data.file.extension, extension);
    EXPECT_EQ(decoded_sample->timestamp, timestamp);
    EXPECT_STREQ(decoded_sample->metric_id, metric_id);
    free(decoded_sample);
    destroy_sample_sender(sender);

    // Clean up test file
    unlink(filepath);
}

#ifdef BCP_FETCH_BOUNDS_CHECKING
TEST_F(SendSampleTest, InvalidMetricId)
{
    // Create a metric_id that exceeds the maximum size
    char long_metric_id[METRIC_ID_MAX_SIZE + 1];
    memset(long_metric_id, 'a', sizeof(long_metric_id) - 1);
    long_metric_id[sizeof(long_metric_id) - 1] = '\0';

    sender_params.metric_id = long_metric_id;
    sample_sender_status_t status;
    sample_sender_int32_t* sender =
        make_sample_sender_int32(sender_params, &status);
    EXPECT_EQ(status, SAMPLE_SENDER_STATUS_INVALID_PARAMETER);
    EXPECT_EQ(sender, nullptr);
}
#endif

TEST_F(SendSampleTest, InvalidSocket)
{
    sender_params.metric_id = "test";
    sender_params.node = "invalid-host";
    sender_params.service = "0";

    sample_sender_status_t status;
    sample_sender_int32_t* sender =
        make_sample_sender_int32(sender_params, &status);
    EXPECT_EQ(status, SAMPLE_SENDER_STATUS_SOCKET_CREATION_FAILED);
    EXPECT_EQ(sender, nullptr);
}

TEST_F(SendSampleTest, NullMetricId)
{
    sender_params.metric_id = nullptr;
    sample_sender_status_t status;
    sample_sender_int32_t* sender =
        make_sample_sender_int32(sender_params, &status);
    EXPECT_EQ(status, SAMPLE_SENDER_STATUS_INVALID_PARAMETER);
    EXPECT_EQ(sender, nullptr);
}
