#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class AccessDeniedException : public PlatformException {
public:
    explicit AccessDeniedException(const std::string& action)
        : PlatformException("Access denied for action: " + action) {}
};

} // namespace online_learning
