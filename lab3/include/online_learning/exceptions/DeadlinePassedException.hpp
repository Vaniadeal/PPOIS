#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class DeadlinePassedException : public PlatformException {
public:
    explicit DeadlinePassedException(const std::string& assignmentId)
        : PlatformException("Deadline has passed for assignment: " + assignmentId) {}
};

} // namespace online_learning
