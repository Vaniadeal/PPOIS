#include "online_learning/progress/LessonProgress.hpp"

namespace online_learning {

LessonProgress::LessonProgress()
    : progressId_(""), enrollmentId_(""), lessonId_(""),
      isCompleted_(false), timeSpentSeconds_(0), lastAccessedTimestamp_(0) {}

LessonProgress::LessonProgress(std::string progressId, std::string enrollmentId,
                               std::string lessonId, int64_t accessTimestamp)
    : progressId_(std::move(progressId)), enrollmentId_(std::move(enrollmentId)),
      lessonId_(std::move(lessonId)), isCompleted_(false),
      timeSpentSeconds_(0), lastAccessedTimestamp_(accessTimestamp) {}

void LessonProgress::markCompleted(int64_t completionTimestamp) {
    isCompleted_ = true;
    lastAccessedTimestamp_ = completionTimestamp;
}

void LessonProgress::addTimeSpent(int seconds) {
    if (seconds > 0) {
        timeSpentSeconds_ += seconds;
    }
}

bool LessonProgress::hasMinimumTimeSpent(int thresholdSeconds) const {
    return timeSpentSeconds_ >= thresholdSeconds;
}

std::string LessonProgress::formatProgressStatus() const {
    std::string status = isCompleted_ ? "Completed" : "In Progress";
    return "Lesson: " + lessonId_ + " [" + status + "], Time: " + std::to_string(timeSpentSeconds_) + "s";
}

} // namespace online_learning
