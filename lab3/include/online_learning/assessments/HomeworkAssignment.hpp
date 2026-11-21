#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class HomeworkAssignment {
private:
    std::string assignmentId_;
    std::string lessonId_;
    std::string taskPrompt_;
    int maxScore_;
    int64_t deadlineTimestamp_;
    bool allowLateSubmissions_;

public:
    HomeworkAssignment();
    HomeworkAssignment(std::string assignmentId, std::string lessonId,
                       std::string prompt, int maxScore, int64_t deadline,
                       bool allowLate = false);

    bool isDeadlinePassed(int64_t currentTimestamp) const;
    void extendDeadline(int64_t additionalSeconds);
    void updatePrompt(const std::string& newPrompt);
    int64_t getTimeRemainingSeconds(int64_t currentTimestamp) const;

    const std::string& getAssignmentId() const { return assignmentId_; }
    const std::string& getLessonId() const { return lessonId_; }
    const std::string& getTaskPrompt() const { return taskPrompt_; }
    int getMaxScore() const { return maxScore_; }
    int64_t getDeadlineTimestamp() const { return deadlineTimestamp_; }
    bool allowsLateSubmissions() const { return allowLateSubmissions_; }
};

} // namespace online_learning
