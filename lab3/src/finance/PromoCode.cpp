#include "online_learning/finance/PromoCode.hpp"

namespace online_learning {

PromoCode::PromoCode()
    : code_(""), discountPercentage_(0.0), expirationDate_(0),
      maxUses_(0), currentUsesCount_(0), isActive_(false) {}

PromoCode::PromoCode(std::string code, double discountPercent, int64_t expiration, int maxUses)
    : code_(std::move(code)), discountPercentage_(discountPercent),
      expirationDate_(expiration), maxUses_(maxUses),
      currentUsesCount_(0), isActive_(true) {}

bool PromoCode::isValid(int64_t currentTimestamp) const {
    if (!isActive_ || currentUsesCount_ >= maxUses_) {
        return false;
    }
    return currentTimestamp <= expirationDate_;
}

void PromoCode::applyDiscount() {
    currentUsesCount_++;
    if (currentUsesCount_ >= maxUses_) {
        isActive_ = false;
    }
}

double PromoCode::calculateDiscountAmount(double basePrice) const {
    return (basePrice * discountPercentage_) / 100.0;
}

void PromoCode::deactivate() {
    isActive_ = false;
}

} // namespace online_learning
