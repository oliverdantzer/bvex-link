#include "onboard_telemetry_recv_server.hpp"
#include "recv_server.hpp"
#include <codec/onboard-tm/decode.hpp>
#include <iostream>
#include <memory>

OnboardTelemetryRecvServer::OnboardTelemetryRecvServer(
    boost::asio::io_service& io_service, boost::asio::ip::port_type port,
    Command& command, std::size_t buffer_size)
    : recv_server_(io_service, port,
                   std::bind(&OnboardTelemetryRecvServer::handle_message, this,
                             std::placeholders::_1),
                   buffer_size),
      command_(command) {};

std::unique_ptr<SampleData> sample_struct_to_sample_data(Sample sample)
{
    std::string metric_id(sample.metric_id);
    float timestamp = sample.timestamp;
    SampleMetadata metadata = {.metric_id = metric_id, .timestamp = timestamp};

    std::unique_ptr<SampleData> sample_data;

    switch(sample.which_data) {
    case Sample_primitive_tag:
        switch(sample.data.primitive.which_value) {
        case primitive_Primitive_int_val_tag:
            return std::make_unique<PrimitiveSample>(
                metadata, sample.data.primitive.value.int_val);
        case primitive_Primitive_float_val_tag:
            return std::make_unique<PrimitiveSample>(
                metadata, sample.data.primitive.value.float_val);
        case primitive_Primitive_double_val_tag:
            return std::make_unique<PrimitiveSample>(
                metadata, sample.data.primitive.value.double_val);
        default:
            std::cerr << "Unknown/unimplemented value type" << std::endl;
            return nullptr;
        }
    case Sample_file_tag:
        return std::make_unique<FileSample>(metadata, sample.data.file.filepath,
                                            sample.data.file.extension);
    default:
        std::cerr << "Unknown/unimplemented data type" << std::endl;
        return nullptr;
    }
}

void OnboardTelemetryRecvServer::handle_message(
    std::unique_ptr<std::vector<uint8_t>> message)
{
    std::optional<Sample> sample = decode_payload(*message);
    if(sample) {
        command_.add_sample(sample_struct_to_sample_data(*sample));
    }
}
