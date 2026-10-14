#pragma once
#include <string>

namespace online_learning {

class Question {
protected:
    std::string questionId_;
    std::string quizId_;
    std::string text_;
    int points_;
    std::string questionType_;

public:
    Question(std::string questionId, std::string quizId, std::string text,
             int points, std::string questionType);
    virtual ~Question() = default;

    virtual bool evaluateAnswer(const std::string& answer) const = 0;
    void updatePoints(int newPoints);
    std::string getSummary() const;

    const std::string& getQuestionId() const { return questionId_; }
    const std::string& getQuizId() const { return quizId_; }
    const std::string& getText() const { return text_; }
    int getPoints() const { return points_; }
    const std::string& getQuestionType() const { return questionType_; }
};

} // namespace online_learning
