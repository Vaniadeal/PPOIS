#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class StudentBadgeAward {
private:
    std::string awardId_;
    std::string studentId_;
    std::string badgeId_;
    int64_t awardedDate_;

public:
    StudentBadgeAward();
    StudentBadgeAward(std::string awardId, std::string studentId,
                      std::string badgeId, int64_t awardedDate);

    bool matchesStudent(const std::string& studentId) const;
    std::string formatAwardLog() const;

    const std::string& getAwardId() const { return awardId_; }
    const std::string& getStudentId() const { return studentId_; }
    const std::string& getBadgeId() const { return badgeId_; }
    int64_t getAwardedDate() const { return awardedDate_; }
};

} // namespace online_learning
