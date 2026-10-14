#pragma once
#include <vector>
#include <cstdint>
#include "online_learning/assessments/Question.hpp"

namespace online_learning {

class MultipleChoiceQuestion : public Question {
private:
    std::vector<std::string> options_;
    uint32_t correctOptionsMask_;

public:
    MultipleChoiceQuestion(std::string questionId, std::string quizId,
                           std::string text, int points,
                           std::vector<std::string> options, uint32_t correctMask);

    bool evaluateAnswer(const std::string& answerMaskStr) const override;
    bool isOptionCorrect(int optionIndex) const;
    int getCorrectCount() const;

    const std::vector<std::string>& getOptions() const { return options_; }
    uint32_t getCorrectOptionsMask() const { return correctOptionsMask_; }
};

} // namespace online_learning
