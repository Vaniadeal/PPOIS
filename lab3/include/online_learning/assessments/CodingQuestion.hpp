#pragma once
#include <string>
#include "online_learning/assessments/Question.hpp"

namespace online_learning {

class CodingQuestion : public Question {
private:
    std::string starterCode_;
    std::string expectedOutput_;
    int timeoutMilliseconds_;

public:
    CodingQuestion(std::string questionId, std::string quizId, std::string text,
                   int points, std::string starterCode, std::string expectedOutput,
                   int timeoutMs);

    bool evaluateAnswer(const std::string& codeOutput) const override;
    void updateStarterCode(const std::string& newStarter);
    bool isOutputExactMatch(const std::string& candidateOutput) const;

    const std::string& getStarterCode() const { return starterCode_; }
    const std::string& getExpectedOutput() const { return expectedOutput_; }
    int getTimeoutMilliseconds() const { return timeoutMilliseconds_; }
};

} // namespace online_learning
