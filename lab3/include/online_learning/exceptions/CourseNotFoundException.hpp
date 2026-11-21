#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class CourseNotFoundException : public PlatformException {
public:
    explicit CourseNotFoundException(const std::string& courseId)
        : PlatformException("Course not found with ID: " + courseId) {}
};

} // namespace online_learning
