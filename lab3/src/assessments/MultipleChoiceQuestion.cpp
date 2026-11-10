#include "online_learning/assessments/MultipleChoiceQuestion.hpp"

namespace online_learning {

MultipleChoiceQuestion::MultipleChoiceQuestion(std::string questionId, std::string quizId,
                                               std::string text, int points,
                                               std::vector<std::string> options, uint32_t correctMask)
    : Question(std::move(questionId), std::move(quizId), std::move(text), points, "MultipleChoice"),
      options_(std::move(options)), correctOptionsMask_(correctMask) {}

bool MultipleChoiceQuestion::evaluateAnswer(const std::string& answerMaskStr) const {
    try {
        uint32_t candidateMask = static_cast<uint32_t>(std::stoul(answerMaskStr));
        return candidateMask == correctOptionsMask_;
    } catch (...) {
        return false;
    }
}

bool MultipleChoiceQuestion::isOptionCorrect(int optionIndex) const {
    if (optionIndex < 0 || optionIndex >= 32) {
        return false;
    }
    return (correctOptionsMask_ & (1u << optionIndex)) != 0;
}

int MultipleChoiceQuestion::getCorrectCount() const {
    int count = 0;
    uint32_t mask = correctOptionsMask_;
    while (mask > 0) {
        count += (mask & 1);
        mask >>= 1;
    }
    return count;
}

} // namespace online_learning
