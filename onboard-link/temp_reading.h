#ifndef TEMP_READING_H
#define TEMP_READING_H

#include <string>
#include <ctime>

class Sample {
public:
    std::string name;
    std::time_t timestamp;
    std::string value;

    Sample(const std::string& name, std::time_t timestamp, const std::string& value);
};

Sample get_temp_reading();

#endif // TEMP_READING_H