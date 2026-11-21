#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class UserNotFoundException : public PlatformException {
public:
    explicit UserNotFoundException(const std::string& identifier)
        : PlatformException("User not found: " + identifier) {}
};

} // namespace online_learning
