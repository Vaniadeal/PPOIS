#include "online_learning/assessments/HomeworkAssignment.hpp"

namespace online_learning {

HomeworkAssignment::HomeworkAssignment()
    : assignmentId_(""), lessonId_(""), taskPrompt_(""), maxScore_(100),
      deadlineTimestamp_(0), allowLateSubmissions_(false) {}

HomeworkAssignment::HomeworkAssignment(std::string assignmentId, std::string lessonId,
                                       std::string prompt, int maxScore, int64_t deadline,
                                       bool allowLate)
    : assignmentId_(std::move(assignmentId)), lessonId_(std::move(lessonId)),
      taskPrompt_(std::move(prompt)), maxScore_(maxScore),
      deadlineTimestamp_(deadline), allowLateSubmissions_(allowLate) {}

bool HomeworkAssignment::isDeadlinePassed(int64_t currentTimestamp) const {
    return currentTimestamp > deadlineTimestamp_;
}

void HomeworkAssignment::extendDeadline(int64_t additionalSeconds) {
    deadlineTimestamp_ += additionalSeconds;
}

void HomeworkAssignment::updatePrompt(const std::string& newPrompt) {
    taskPrompt_ = newPrompt;
}

int64_t HomeworkAssignment::getTimeRemainingSeconds(int64_t currentTimestamp) const {
    if (currentTimestamp < deadlineTimestamp_) {
        return deadlineTimestamp_ - currentTimestamp;
    }
    return 0;
}

} // namespace online_learning
