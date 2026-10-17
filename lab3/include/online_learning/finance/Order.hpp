#pragma once
#include <string>
#include "online_learning/finance/PromoCode.hpp"
#include "online_learning/exceptions/InvalidPromoCodeException.hpp"

namespace online_learning {

class Order {
public:
    enum class OrderStatus { Created, Paid, Cancelled, Refunded };

private:
    std::string orderId_;
    std::string studentId_;
    std::string courseId_;
    double originalPrice_;
    double discountAmount_;
    double finalPrice_;
    OrderStatus status_;

public:
    Order();
    Order(std::string orderId, std::string studentId, std::string courseId, double originalPrice);

    void applyPromoCode(const PromoCode& promo, int64_t currentTimestamp);
    void markPaid();
    void cancelOrder();
    bool isPaid() const;
    std::string formatReceipt() const;

    const std::string& getOrderId() const { return orderId_; }
    const std::string& getStudentId() const { return studentId_; }
    const std::string& getCourseId() const { return courseId_; }
    double getOriginalPrice() const { return originalPrice_; }
    double getDiscountAmount() const { return discountAmount_; }
    double getFinalPrice() const { return finalPrice_; }
    OrderStatus getStatus() const { return status_; }
};

} // namespace online_learning
