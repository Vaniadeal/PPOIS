#include "online_learning/assessments/CodingQuestion.hpp"

namespace online_learning {

CodingQuestion::CodingQuestion(std::string questionId, std::string quizId, std::string text,
                               int points, std::string starterCode, std::string expectedOutput,
                               int timeoutMs)
    : Question(std::move(questionId), std::move(quizId), std::move(text), points, "Coding"),
      starterCode_(std::move(starterCode)), expectedOutput_(std::move(expectedOutput)),
      timeoutMilliseconds_(timeoutMs) {}

bool CodingQuestion::evaluateAnswer(const std::string& codeOutput) const {
    return isOutputExactMatch(codeOutput);
}

void CodingQuestion::updateStarterCode(const std::string& newStarter) {
    starterCode_ = newStarter;
}

bool CodingQuestion::isOutputExactMatch(const std::string& candidateOutput) const {
    return candidateOutput == expectedOutput_;
}

} // namespace online_learning
