#pragma once
#include <string>

namespace online_learning {

class StudentAnswer {
private:
    std::string answerId_;
    std::string attemptId_;
    std::string questionId_;
    std::string chosenAnswerText_;
    bool isCorrect_;
    int earnedPoints_;

public:
    StudentAnswer();
    StudentAnswer(std::string answerId, std::string attemptId,
                  std::string questionId, std::string chosenText);

    void grade(bool correct, int points);
    bool hasEarnedPoints() const;
    std::string formatAnswerResult() const;

    const std::string& getAnswerId() const { return answerId_; }
    const std::string& getAttemptId() const { return attemptId_; }
    const std::string& getQuestionId() const { return questionId_; }
    const std::string& getChosenAnswerText() const { return chosenAnswerText_; }
    bool isCorrect() const { return isCorrect_; }
    int getEarnedPoints() const { return earnedPoints_; }
};

} // namespace online_learning
