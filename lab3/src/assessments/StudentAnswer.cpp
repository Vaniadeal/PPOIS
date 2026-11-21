#include "online_learning/assessments/StudentAnswer.hpp"

namespace online_learning {

StudentAnswer::StudentAnswer()
    : answerId_(""), attemptId_(""), questionId_(""), chosenAnswerText_(""),
      isCorrect_(false), earnedPoints_(0) {}

StudentAnswer::StudentAnswer(std::string answerId, std::string attemptId,
                             std::string questionId, std::string chosenText)
    : answerId_(std::move(answerId)), attemptId_(std::move(attemptId)),
      questionId_(std::move(questionId)), chosenAnswerText_(std::move(chosenText)),
      isCorrect_(false), earnedPoints_(0) {}

void StudentAnswer::grade(bool correct, int points) {
    isCorrect_ = correct;
    earnedPoints_ = correct ? points : 0;
}

bool StudentAnswer::hasEarnedPoints() const {
    return earnedPoints_ > 0;
}

std::string StudentAnswer::formatAnswerResult() const {
    return isCorrect_ ? "CORRECT (+ " + std::to_string(earnedPoints_) + " pts)" : "INCORRECT (0 pts)";
}

} // namespace online_learning
