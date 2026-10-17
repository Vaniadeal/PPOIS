#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class RefundRequest {
private:
    std::string refundId_;
    std::string orderId_;
    std::string reason_;
    int64_t requestDate_;
    bool isApproved_;
    double refundAmount_;

public:
    RefundRequest();
    RefundRequest(std::string refundId, std::string orderId, std::string reason,
                  int64_t requestDate, double amount);

    void approve();
    void reject();
    bool isEligibleForAutoApproval(int64_t orderDate, int64_t currentDate) const;
    std::string formatSummary() const;

    const std::string& getRefundId() const { return refundId_; }
    const std::string& getOrderId() const { return orderId_; }
    const std::string& getReason() const { return reason_; }
    int64_t getRequestDate() const { return requestDate_; }
    bool isApproved() const { return isApproved_; }
    double getRefundAmount() const { return refundAmount_; }
};

} // namespace online_learning
