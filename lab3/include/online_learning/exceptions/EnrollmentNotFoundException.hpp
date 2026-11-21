#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class EnrollmentNotFoundException : public PlatformException {
public:
    explicit EnrollmentNotFoundException(const std::string& studentId, const std::string& courseId)
        : PlatformException("Enrollment not found for student " + studentId + " in course " + courseId) {}
};

} // namespace online_learning
