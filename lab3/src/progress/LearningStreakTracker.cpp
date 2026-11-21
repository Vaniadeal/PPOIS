#include "online_learning/progress/LearningStreakTracker.hpp"
#include <algorithm>

namespace online_learning {

LearningStreakTracker::LearningStreakTracker()
    : studentId_(""), currentStreakDays_(0), longestStreakDays_(0), lastActivityDate_(0) {}

LearningStreakTracker::LearningStreakTracker(std::string studentId)
    : studentId_(std::move(studentId)), currentStreakDays_(0),
      longestStreakDays_(0), lastActivityDate_(0) {}

void LearningStreakTracker::recordDailyActivity(int64_t todayTimestamp) {
    constexpr int64_t kSecondsInDay = 86400;
    if (lastActivityDate_ == 0) {
        currentStreakDays_ = 1;
    } else {
        int64_t diff = todayTimestamp - lastActivityDate_;
        if (diff <= kSecondsInDay * 2) {
            currentStreakDays_++;
        } else {
            currentStreakDays_ = 1;
        }
    }
    longestStreakDays_ = std::max(longestStreakDays_, currentStreakDays_);
    lastActivityDate_ = todayTimestamp;
}

void LearningStreakTracker::resetStreak() {
    currentStreakDays_ = 0;
}

bool LearningStreakTracker::isStreakActive(int64_t currentTimestamp) const {
    constexpr int64_t kSecondsInDay = 86400;
    return (currentTimestamp - lastActivityDate_) <= (kSecondsInDay * 2);
}

std::string LearningStreakTracker::getStreakStatusMessage() const {
    return "Current Streak: " + std::to_string(currentStreakDays_) + " days (Best: " +
           std::to_string(longestStreakDays_) + " days)";
}

} // namespace online_learning
