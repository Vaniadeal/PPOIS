#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class PromoCode {
private:
    std::string code_;
    double discountPercentage_;
    int64_t expirationDate_;
    int maxUses_;
    int currentUsesCount_;
    bool isActive_;

public:
    PromoCode();
    PromoCode(std::string code, double discountPercent, int64_t expiration, int maxUses);

    bool isValid(int64_t currentTimestamp) const;
    void applyDiscount();
    double calculateDiscountAmount(double basePrice) const;
    void deactivate();

    const std::string& getCode() const { return code_; }
    double getDiscountPercentage() const { return discountPercentage_; }
    int64_t getExpirationDate() const { return expirationDate_; }
    int getMaxUses() const { return maxUses_; }
    int getCurrentUsesCount() const { return currentUsesCount_; }
    bool isActive() const { return isActive_; }
};

} // namespace online_learning
