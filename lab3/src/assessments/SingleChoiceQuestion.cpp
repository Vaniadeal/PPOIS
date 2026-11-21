#include "online_learning/assessments/SingleChoiceQuestion.hpp"

namespace online_learning {

SingleChoiceQuestion::SingleChoiceQuestion(std::string questionId, std::string quizId,
                                           std::string text, int points,
                                           std::vector<std::string> options, int correctIndex)
    : Question(std::move(questionId), std::move(quizId), std::move(text), points, "SingleChoice"),
      options_(std::move(options)), correctOptionIndex_(correctIndex) {}

bool SingleChoiceQuestion::evaluateAnswer(const std::string& answer) const {
    try {
        int selectedIndex = std::stoi(answer);
        return selectedIndex == correctOptionIndex_;
    } catch (...) {
        return false;
    }
}

void SingleChoiceQuestion::addOption(const std::string& option) {
    options_.push_back(option);
}

int SingleChoiceQuestion::getOptionsCount() const {
    return static_cast<int>(options_.size());
}

} // namespace online_learning
