#include "online_learning/certificates/StudentBadgeAward.hpp"

namespace online_learning {

StudentBadgeAward::StudentBadgeAward()
    : awardId_(""), studentId_(""), badgeId_(""), awardedDate_(0) {}

StudentBadgeAward::StudentBadgeAward(std::string awardId, std::string studentId,
                                     std::string badgeId, int64_t awardedDate)
    : awardId_(std::move(awardId)), studentId_(std::move(studentId)),
      badgeId_(std::move(badgeId)), awardedDate_(awardedDate) {}

bool StudentBadgeAward::matchesStudent(const std::string& studentId) const {
    return studentId_ == studentId;
}

std::string StudentBadgeAward::formatAwardLog() const {
    return "Award #" + awardId_ + ": Badge " + badgeId_ + " awarded to " + studentId_;
}

} // namespace online_learning
