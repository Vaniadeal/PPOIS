#include "online_learning/assessments/QuizAttempt.hpp"

namespace online_learning {

QuizAttempt::QuizAttempt()
    : attemptId_(""), studentId_(""), quizId_(""), scorePercentage_(0),
      isPassed_(false), startTime_(0), submitTime_(0) {}

QuizAttempt::QuizAttempt(std::string attemptId, std::string studentId, std::string quizId, int64_t startTime)
    : attemptId_(std::move(attemptId)), studentId_(std::move(studentId)),
      quizId_(std::move(quizId)), scorePercentage_(0), isPassed_(false),
      startTime_(startTime), submitTime_(0) {}

void QuizAttempt::recordAnswer(const StudentAnswer& answer) {
    answers_.push_back(answer);
}

void QuizAttempt::finalizeAttempt(int totalScore, bool passed, int64_t submitTimestamp) {
    scorePercentage_ = totalScore;
    isPassed_ = passed;
    submitTime_ = submitTimestamp;
}

int QuizAttempt::calculateTotalEarnedPoints() const {
    int total = 0;
    for (const auto& ans : answers_) {
        total += ans.getEarnedPoints();
    }
    return total;
}

int64_t QuizAttempt::getDurationSeconds() const {
    if (submitTime_ >= startTime_) {
        return submitTime_ - startTime_;
    }
    return 0;
}

} // namespace online_learning
