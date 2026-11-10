#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class InsufficientFundsException : public PlatformException {
public:
    InsufficientFundsException(double available, double requiredAmount)
        : PlatformException("Insufficient funds: available $" + std::to_string(available) +
                            ", required $" + std::to_string(requiredAmount)) {}
};

} // namespace online_learning
