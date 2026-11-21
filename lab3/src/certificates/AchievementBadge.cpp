#include "online_learning/certificates/AchievementBadge.hpp"

namespace online_learning {

AchievementBadge::AchievementBadge()
    : badgeId_(""), title_(""), description_(""), iconUrl_(""), requiredMilestoneValue_(0) {}

AchievementBadge::AchievementBadge(std::string badgeId, std::string title,
                                   std::string description, std::string iconUrl, int requiredValue)
    : badgeId_(std::move(badgeId)), title_(std::move(title)),
      description_(std::move(description)), iconUrl_(std::move(iconUrl)),
      requiredMilestoneValue_(requiredValue) {}

bool AchievementBadge::isEligible(int studentMetricValue) const {
    return studentMetricValue >= requiredMilestoneValue_;
}

void AchievementBadge::updateDescription(const std::string& newDesc) {
    description_ = newDesc;
}

std::string AchievementBadge::formatBadgeBanner() const {
    return "[BADGE: " + title_ + "] " + description_;
}

} // namespace online_learning
