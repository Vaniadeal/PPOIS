#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class LearningStreakTracker {
private:
    std::string studentId_;
    int currentStreakDays_;
    int longestStreakDays_;
    int64_t lastActivityDate_;

public:
    LearningStreakTracker();
    explicit LearningStreakTracker(std::string studentId);

    void recordDailyActivity(int64_t todayTimestamp);
    void resetStreak();
    bool isStreakActive(int64_t currentTimestamp) const;
    std::string getStreakStatusMessage() const;

    const std::string& getStudentId() const { return studentId_; }
    int getCurrentStreakDays() const { return currentStreakDays_; }
    int getLongestStreakDays() const { return longestStreakDays_; }
    int64_t getLastActivityDate() const { return lastActivityDate_; }
};

} // namespace online_learning
