#include "online_learning/progress/Enrollment.hpp"
#include <algorithm>

namespace online_learning {

Enrollment::Enrollment()
    : enrollmentId_(""), studentId_(""), courseId_(""),
      enrolledDate_(0), completionDate_(0), progressPercent_(0.0), status_(Status::Active) {}

Enrollment::Enrollment(std::string enrollmentId, std::string studentId,
                       std::string courseId, int64_t enrolledDate)
    : enrollmentId_(std::move(enrollmentId)), studentId_(std::move(studentId)),
      courseId_(std::move(courseId)), enrolledDate_(enrolledDate),
      completionDate_(0), progressPercent_(0.0), status_(Status::Active) {}

void Enrollment::updateProgress(double newPercent) {
    progressPercent_ = std::clamp(newPercent, 0.0, 100.0);
    if (progressPercent_ >= 100.0 && status_ == Status::Active) {
        status_ = Status::Completed;
    }
}

void Enrollment::completeCourse(int64_t completionDate) {
    progressPercent_ = 100.0;
    completionDate_ = completionDate;
    status_ = Status::Completed;
}

void Enrollment::dropCourse() {
    status_ = Status::Dropped;
}

bool Enrollment::isCompleted() const {
    return status_ == Status::Completed;
}

bool Enrollment::isActive() const {
    return status_ == Status::Active;
}

std::string Enrollment::getStatusString() const {
    switch (status_) {
        case Status::Active: return "Active";
        case Status::Completed: return "Completed";
        case Status::Dropped: return "Dropped";
        case Status::Suspended: return "Suspended";
    }
    return "Unknown";
}

} // namespace online_learning
