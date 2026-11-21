#include "online_learning/courses/CourseModule.hpp"
#include <algorithm>

namespace online_learning {

CourseModule::CourseModule()
    : moduleId_(""), courseId_(""), title_(""), orderIndex_(0),
      isMandatory_(true), passingScorePercentage_(70.0) {}

CourseModule::CourseModule(std::string moduleId, std::string courseId, std::string title,
                           int orderIndex, bool isMandatory, double passingScore)
    : moduleId_(std::move(moduleId)), courseId_(std::move(courseId)),
      title_(std::move(title)), orderIndex_(orderIndex),
      isMandatory_(isMandatory), passingScorePercentage_(passingScore) {}

void CourseModule::addLessonId(const std::string& lessonId) {
    lessonIds_.push_back(lessonId);
}

void CourseModule::removeLessonId(const std::string& lessonId) {
    auto it = std::remove(lessonIds_.begin(), lessonIds_.end(), lessonId);
    lessonIds_.erase(it, lessonIds_.end());
}

void CourseModule::updatePassingScore(double newPassingScore) {
    passingScorePercentage_ = newPassingScore;
}

int CourseModule::getTotalLessonsCount() const {
    return static_cast<int>(lessonIds_.size());
}

bool CourseModule::isEligibleForCompletion(double studentScore) const {
    return studentScore >= passingScorePercentage_;
}

} // namespace online_learning
