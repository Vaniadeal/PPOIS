#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class UserAlreadyExistsException : public PlatformException {
public:
    explicit UserAlreadyExistsException(const std::string& email)
        : PlatformException("User already exists with email: " + email) {}
};

} // namespace online_learning
