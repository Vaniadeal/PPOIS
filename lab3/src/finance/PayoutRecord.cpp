#include "online_learning/finance/PayoutRecord.hpp"

namespace online_learning {

PayoutRecord::PayoutRecord()
    : payoutId_(""), instructorId_(""), grossAmount_(0.0),
      platformCommissionRate_(0.2), payoutDate_(0), isProcessed_(false) {}

PayoutRecord::PayoutRecord(std::string payoutId, std::string instructorId, double grossAmount,
                           double commissionRate, int64_t payoutDate)
    : payoutId_(std::move(payoutId)), instructorId_(std::move(instructorId)),
      grossAmount_(grossAmount), platformCommissionRate_(commissionRate),
      payoutDate_(payoutDate), isProcessed_(false) {}

void PayoutRecord::processPayout() {
    isProcessed_ = true;
}

double PayoutRecord::calculateNetAmount() const {
    return grossAmount_ * (1.0 - platformCommissionRate_);
}

double PayoutRecord::calculateCommissionFee() const {
    return grossAmount_ * platformCommissionRate_;
}

std::string PayoutRecord::formatPayoutReceipt() const {
    return "Payout #" + payoutId_ + " to " + instructorId_ + ": Net $" +
           std::to_string(calculateNetAmount()) + " (Fee: $" +
           std::to_string(calculateCommissionFee()) + ")";
}

} // namespace online_learning
