#pragma once
#include <string>
#include <vector>

namespace online_learning {

class Quiz {
private:
    std::string quizId_;
    std::string lessonId_;
    std::string title_;
    int timeLimitMinutes_;
    int maxAttempts_;
    int passingPercentage_;
    std::vector<std::string> questionIds_;

public:
    Quiz();
    Quiz(std::string quizId, std::string lessonId, std::string title,
         int timeLimitMinutes, int maxAttempts, int passingPercentage);

    void addQuestionId(const std::string& questionId);
    bool canStudentAttempt(int previousAttemptsCount) const;
    bool isScorePassing(int scorePercentage) const;
    int getTotalQuestionsCount() const;

    const std::string& getQuizId() const { return quizId_; }
    const std::string& getId() const { return quizId_; }
    const std::string& getLessonId() const { return lessonId_; }
    const std::string& getTitle() const { return title_; }
    int getTimeLimitMinutes() const { return timeLimitMinutes_; }
    int getMaxAttempts() const { return maxAttempts_; }
    int getPassingPercentage() const { return passingPercentage_; }
    const std::vector<std::string>& getQuestionIds() const { return questionIds_; }
};

} // namespace online_learning
