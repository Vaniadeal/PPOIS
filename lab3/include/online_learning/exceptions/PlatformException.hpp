#pragma once
#include <stdexcept>
#include <string>

namespace online_learning {

class PlatformException : public std::runtime_error {
public:
    explicit PlatformException(const std::string& message)
        : std::runtime_error(message) {}
};

} // namespace online_learning
