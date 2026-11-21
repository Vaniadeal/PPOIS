#include "online_learning/users/TeachingAssistant.hpp"
#include <algorithm>

namespace online_learning {

TeachingAssistant::TeachingAssistant(std::string id, std::string email, std::string fullName, int64_t regDate)
    : User(std::move(id), std::move(email), std::move(fullName), regDate),
      reviewedSubmissionsCount_(0), canGradeQuizzes_(true), canAnswerForum_(true) {}

void TeachingAssistant::assignToCourse(const std::string& courseId) {
    assignedCourseIds_.push_back(courseId);
}

void TeachingAssistant::unassignFromCourse(const std::string& courseId) {
    auto it = std::remove(assignedCourseIds_.begin(), assignedCourseIds_.end(), courseId);
    assignedCourseIds_.erase(it, assignedCourseIds_.end());
}

void TeachingAssistant::recordSubmissionReviewed() {
    reviewedSubmissionsCount_++;
}

bool TeachingAssistant::hasPermissionForCourse(const std::string& courseId) const {
    return std::find(assignedCourseIds_.begin(), assignedCourseIds_.end(), courseId) != assignedCourseIds_.end();
}

} // namespace online_learning
