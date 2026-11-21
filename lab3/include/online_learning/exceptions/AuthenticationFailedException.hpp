#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class AuthenticationFailedException : public PlatformException {
public:
    explicit AuthenticationFailedException(const std::string& reason)
        : PlatformException("Authentication failed: " + reason) {}
};

} // namespace online_learning
