#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class CourseAlreadyPublishedException : public PlatformException {
public:
    explicit CourseAlreadyPublishedException(const std::string& courseId)
        : PlatformException("Course is already published and cannot be modified: " + courseId) {}
};

} // namespace online_learning
