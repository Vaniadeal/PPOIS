#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class LessonProgress {
private:
    std::string progressId_;
    std::string enrollmentId_;
    std::string lessonId_;
    bool isCompleted_;
    int timeSpentSeconds_;
    int64_t lastAccessedTimestamp_;

public:
    LessonProgress();
    LessonProgress(std::string progressId, std::string enrollmentId,
                   std::string lessonId, int64_t accessTimestamp);

    void markCompleted(int64_t completionTimestamp);
    void addTimeSpent(int seconds);
    bool hasMinimumTimeSpent(int thresholdSeconds) const;
    std::string formatProgressStatus() const;

    const std::string& getProgressId() const { return progressId_; }
    const std::string& getEnrollmentId() const { return enrollmentId_; }
    const std::string& getLessonId() const { return lessonId_; }
    bool isCompleted() const { return isCompleted_; }
    int getTimeSpentSeconds() const { return timeSpentSeconds_; }
    int64_t getLastAccessedTimestamp() const { return lastAccessedTimestamp_; }
};

} // namespace online_learning
