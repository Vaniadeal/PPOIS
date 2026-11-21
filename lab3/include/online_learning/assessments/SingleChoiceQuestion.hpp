#pragma once
#include <vector>
#include "online_learning/assessments/Question.hpp"

namespace online_learning {

class SingleChoiceQuestion : public Question {
private:
    std::vector<std::string> options_;
    int correctOptionIndex_;

public:
    SingleChoiceQuestion(std::string questionId, std::string quizId,
                         std::string text, int points,
                         std::vector<std::string> options, int correctIndex);

    bool evaluateAnswer(const std::string& answer) const override;
    void addOption(const std::string& option);
    int getOptionsCount() const;

    const std::vector<std::string>& getOptions() const { return options_; }
    int getCorrectOptionIndex() const { return correctOptionIndex_; }
};

} // namespace online_learning
