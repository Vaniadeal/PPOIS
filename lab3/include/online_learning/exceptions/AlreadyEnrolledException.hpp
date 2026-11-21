#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class AlreadyEnrolledException : public PlatformException {
public:
    explicit AlreadyEnrolledException(const std::string& studentId, const std::string& courseId)
        : PlatformException("Student " + studentId + " is already enrolled in course " + courseId) {}
};

} // namespace online_learning
