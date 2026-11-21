#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class PayoutRecord {
private:
    std::string payoutId_;
    std::string instructorId_;
    double grossAmount_;
    double platformCommissionRate_;
    int64_t payoutDate_;
    bool isProcessed_;

public:
    PayoutRecord();
    PayoutRecord(std::string payoutId, std::string instructorId, double grossAmount,
                 double commissionRate, int64_t payoutDate);

    void processPayout();
    double calculateNetAmount() const;
    double calculateCommissionFee() const;
    std::string formatPayoutReceipt() const;

    const std::string& getPayoutId() const { return payoutId_; }
    const std::string& getInstructorId() const { return instructorId_; }
    double getGrossAmount() const { return grossAmount_; }
    double getPlatformCommissionRate() const { return platformCommissionRate_; }
    int64_t getPayoutDate() const { return payoutDate_; }
    bool isProcessed() const { return isProcessed_; }
};

} // namespace online_learning
