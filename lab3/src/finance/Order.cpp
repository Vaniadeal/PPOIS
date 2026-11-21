#include "online_learning/finance/Order.hpp"
#include <algorithm>

namespace online_learning {

Order::Order()
    : orderId_(""), studentId_(""), courseId_(""),
      originalPrice_(0.0), discountAmount_(0.0), finalPrice_(0.0),
      status_(OrderStatus::Created) {}

Order::Order(std::string orderId, std::string studentId, std::string courseId, double originalPrice)
    : orderId_(std::move(orderId)), studentId_(std::move(studentId)),
      courseId_(std::move(courseId)), originalPrice_(originalPrice),
      discountAmount_(0.0), finalPrice_(originalPrice), status_(OrderStatus::Created) {}

void Order::applyPromoCode(const PromoCode& promo, int64_t currentTimestamp) {
    if (!promo.isValid(currentTimestamp)) {
        throw InvalidPromoCodeException(promo.getCode(), "Code is expired or limit reached");
    }
    discountAmount_ = promo.calculateDiscountAmount(originalPrice_);
    finalPrice_ = std::max(0.0, originalPrice_ - discountAmount_);
}

void Order::markPaid() {
    status_ = OrderStatus::Paid;
}

void Order::cancelOrder() {
    status_ = OrderStatus::Cancelled;
}

bool Order::isPaid() const {
    return status_ == OrderStatus::Paid;
}

std::string Order::formatReceipt() const {
    return "Order #" + orderId_ + " | Total: $" + std::to_string(finalPrice_) +
           " (Saved: $" + std::to_string(discountAmount_) + ")";
}

} // namespace online_learning
