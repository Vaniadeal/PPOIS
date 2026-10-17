#pragma once
#include <string>

namespace online_learning {

class AchievementBadge {
private:
    std::string badgeId_;
    std::string title_;
    std::string description_;
    std::string iconUrl_;
    int requiredMilestoneValue_;

public:
    AchievementBadge();
    AchievementBadge(std::string badgeId, std::string title,
                     std::string description, std::string iconUrl, int requiredValue);

    bool isEligible(int studentMetricValue) const;
    void updateDescription(const std::string& newDesc);
    std::string formatBadgeBanner() const;

    const std::string& getBadgeId() const { return badgeId_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getDescription() const { return description_; }
    const std::string& getIconUrl() const { return iconUrl_; }
    int getRequiredMilestoneValue() const { return requiredMilestoneValue_; }
};

} // namespace online_learning
