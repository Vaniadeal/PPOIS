#include "online_learning/finance/UserSubscription.hpp"

namespace online_learning {

UserSubscription::UserSubscription()
    : subscriptionId_(""), userId_(""), planId_(""),
      startDate_(0), renewalDate_(0), autoRenew_(false) {}

UserSubscription::UserSubscription(std::string subId, std::string userId, std::string planId,
                                   int64_t startDate, int64_t renewalDate)
    : subscriptionId_(std::move(subId)), userId_(std::move(userId)),
      planId_(std::move(planId)), startDate_(startDate),
      renewalDate_(renewalDate), autoRenew_(true) {}

bool UserSubscription::isActive(int64_t currentTimestamp) const {
    return currentTimestamp < renewalDate_;
}

void UserSubscription::renew(int64_t additionalDaysInSeconds) {
    renewalDate_ += additionalDaysInSeconds;
}

void UserSubscription::cancelAutoRenewal() {
    autoRenew_ = false;
}

int64_t UserSubscription::getRemainingSeconds(int64_t currentTimestamp) const {
    if (currentTimestamp < renewalDate_) {
        return renewalDate_ - currentTimestamp;
    }
    return 0;
}

std::string UserSubscription::formatStatus() const {
    std::string renewal = autoRenew_ ? "Auto-Renew ON" : "Auto-Renew OFF";
    return "Plan: " + planId_ + " (" + renewal + ")";
}

} // namespace online_learning
