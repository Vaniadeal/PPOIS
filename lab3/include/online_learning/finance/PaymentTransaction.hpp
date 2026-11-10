#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class PaymentTransaction {
public:
    enum class Status { Pending, Success, Failed, Refunded };

private:
    std::string transactionId_;
    std::string userId_;
    double amount_;
    std::string currency_;
    Status status_;
    std::string paymentMethod_;
    int64_t timestamp_;

public:
    PaymentTransaction();
    PaymentTransaction(std::string txId, std::string userId, double amount,
                       std::string currency, std::string method, int64_t timestamp);

    void markSuccessful();
    void markFailed();
    void markRefunded();
    bool isCompleted() const;
    std::string getStatusString() const;

    const std::string& getTransactionId() const { return transactionId_; }
    const std::string& getUserId() const { return userId_; }
    double getAmount() const { return amount_; }
    const std::string& getCurrency() const { return currency_; }
    Status getStatus() const { return status_; }
    const std::string& getPaymentMethod() const { return paymentMethod_; }
    int64_t getTimestamp() const { return timestamp_; }
};

} // namespace online_learning
