#include "online_learning/assessments/Question.hpp"

namespace online_learning {

Question::Question(std::string questionId, std::string quizId, std::string text,
                   int points, std::string questionType)
    : questionId_(std::move(questionId)), quizId_(std::move(quizId)),
      text_(std::move(text)), points_(points), questionType_(std::move(questionType)) {}

void Question::updatePoints(int newPoints) {
    if (newPoints > 0) {
        points_ = newPoints;
    }
}

std::string Question::getSummary() const {
    return "[" + questionType_ + "] " + text_ + " (" + std::to_string(points_) + " pts)";
}

} // namespace online_learning
