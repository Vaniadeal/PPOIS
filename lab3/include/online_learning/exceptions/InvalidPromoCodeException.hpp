#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class InvalidPromoCodeException : public PlatformException {
public:
    explicit InvalidPromoCodeException(const std::string& code, const std::string& reason)
        : PlatformException("Invalid promo code '" + code + "': " + reason) {}
};

} // namespace online_learning
