#ifndef SAMPLE_H
#define SAMPLE_H

#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

typedef std::string MetricId;

struct SampleMetadata {
    MetricId metric_id;    // Should be unique to each metric
    int sample_id;    // Should be unique to each metric's samples
    double timestamp; // Time since last epoch
};

class SampleData
{
  public:
    const SampleMetadata metadata;
    virtual ~SampleData() = default; // Virtual destructor
};

class IntSample : public SampleData
{
  public:
    const int value;
    IntSample(MetricId metric_id, float timestamp, int value);
};

class LongSample : public SampleData
{
  public:
    const int64_t value;
    LongSample(MetricId metric_id, float timestamp, int64_t value);
};

class FloatSample : public SampleData
{
  public:
    float value;
    FloatSample(MetricId metric_id, float timestamp, float value);
};

class DoubleSample : public SampleData
{
  public:
    double value;
    DoubleSample(MetricId metric_id, float timestamp, double value);
};

class BoolSample : public SampleData
{
  public:
    bool value;
    BoolSample(std::string metric_id, float timestamp, bool value);
};

class StringSample : public SampleData
{
  public:
    std::string value;
    StringSample(std::string metric_id, float timestamp, std::string value);
};

#endif // SAMPLE_H
