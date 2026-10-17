#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class UserSubscription {
private:
    std::string subscriptionId_;
    std::string userId_;
    std::string planId_;
    int64_t startDate_;
    int64_t renewalDate_;
    bool autoRenew_;

public:
    UserSubscription();
    UserSubscription(std::string subId, std::string userId, std::string planId,
                     int64_t startDate, int64_t renewalDate);

    bool isActive(int64_t currentTimestamp) const;
    void renew(int64_t additionalDaysInSeconds);
    void cancelAutoRenewal();
    int64_t getRemainingSeconds(int64_t currentTimestamp) const;
    std::string formatStatus() const;

    const std::string& getSubscriptionId() const { return subscriptionId_; }
    const std::string& getUserId() const { return userId_; }
    const std::string& getPlanId() const { return planId_; }
    int64_t getStartDate() const { return startDate_; }
    int64_t getRenewalDate() const { return renewalDate_; }
    bool hasAutoRenew() const { return autoRenew_; }
};

} // namespace online_learning
