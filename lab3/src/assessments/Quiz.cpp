#include "online_learning/assessments/Quiz.hpp"

namespace online_learning {

Quiz::Quiz()
    : quizId_(""), lessonId_(""), title_(""), timeLimitMinutes_(0),
      maxAttempts_(1), passingPercentage_(70) {}

Quiz::Quiz(std::string quizId, std::string lessonId, std::string title,
           int timeLimitMinutes, int maxAttempts, int passingPercentage)
    : quizId_(std::move(quizId)), lessonId_(std::move(lessonId)),
      title_(std::move(title)), timeLimitMinutes_(timeLimitMinutes),
      maxAttempts_(maxAttempts), passingPercentage_(passingPercentage) {}

void Quiz::addQuestionId(const std::string& questionId) {
    questionIds_.push_back(questionId);
}

bool Quiz::canStudentAttempt(int previousAttemptsCount) const {
    return previousAttemptsCount < maxAttempts_;
}

bool Quiz::isScorePassing(int scorePercentage) const {
    return scorePercentage >= passingPercentage_;
}

int Quiz::getTotalQuestionsCount() const {
    return static_cast<int>(questionIds_.size());
}

} // namespace online_learning
