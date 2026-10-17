#include "online_learning/finance/RefundRequest.hpp"

namespace online_learning {

RefundRequest::RefundRequest()
    : refundId_(""), orderId_(""), reason_(""), requestDate_(0),
      isApproved_(false), refundAmount_(0.0) {}

RefundRequest::RefundRequest(std::string refundId, std::string orderId, std::string reason,
                             int64_t requestDate, double amount)
    : refundId_(std::move(refundId)), orderId_(std::move(orderId)),
      reason_(std::move(reason)), requestDate_(requestDate),
      isApproved_(false), refundAmount_(amount) {}

void RefundRequest::approve() {
    isApproved_ = true;
}

void RefundRequest::reject() {
    isApproved_ = false;
}

bool RefundRequest::isEligibleForAutoApproval(int64_t orderDate, int64_t currentDate) const {
    constexpr int64_t k14DaysInSeconds = 14 * 86400;
    return (currentDate - orderDate) <= k14DaysInSeconds;
}

std::string RefundRequest::formatSummary() const {
    std::string status = isApproved_ ? "APPROVED" : "PENDING/REJECTED";
    return "Refund #" + refundId_ + " for Order #" + orderId_ + " ($" +
           std::to_string(refundAmount_) + ") - " + status;
}

} // namespace online_learning
