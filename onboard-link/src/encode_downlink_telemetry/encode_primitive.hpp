#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <variant>
#include <vector>

typedef std::variant<int32_t, int64_t, float, double, bool, std::string>
    PrimitiveValue;

boost::shared_ptr<std::vector<uint8_t>> encode_primitive(const PrimitiveValue value);