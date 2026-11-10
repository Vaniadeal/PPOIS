#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "online_learning/assessments/StudentAnswer.hpp"

namespace online_learning {

class QuizAttempt {
private:
    std::string attemptId_;
    std::string studentId_;
    std::string quizId_;
    int scorePercentage_;
    bool isPassed_;
    int64_t startTime_;
    int64_t submitTime_;
    std::vector<StudentAnswer> answers_;

public:
    QuizAttempt();
    QuizAttempt(std::string attemptId, std::string studentId, std::string quizId, int64_t startTime);

    void recordAnswer(const StudentAnswer& answer);
    void finalizeAttempt(int totalScore, bool passed, int64_t submitTimestamp);
    int calculateTotalEarnedPoints() const;
    int64_t getDurationSeconds() const;

    const std::string& getAttemptId() const { return attemptId_; }
    const std::string& getStudentId() const { return studentId_; }
    const std::string& getQuizId() const { return quizId_; }
    int getScorePercentage() const { return scorePercentage_; }
    bool isPassed() const { return isPassed_; }
    int64_t getStartTime() const { return startTime_; }
    int64_t getSubmitTime() const { return submitTime_; }
    const std::vector<StudentAnswer>& getAnswers() const { return answers_; }
};

} // namespace online_learning
