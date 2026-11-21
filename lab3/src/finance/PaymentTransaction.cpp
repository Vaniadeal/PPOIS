#include "online_learning/finance/PaymentTransaction.hpp"

namespace online_learning {

PaymentTransaction::PaymentTransaction()
    : transactionId_(""), userId_(""), amount_(0.0), currency_("USD"),
      status_(Status::Pending), paymentMethod_("CreditCard"), timestamp_(0) {}

PaymentTransaction::PaymentTransaction(std::string txId, std::string userId, double amount,
                                       std::string currency, std::string method, int64_t timestamp)
    : transactionId_(std::move(txId)), userId_(std::move(userId)), amount_(amount),
      currency_(std::move(currency)), status_(Status::Pending),
      paymentMethod_(std::move(method)), timestamp_(timestamp) {}

void PaymentTransaction::markSuccessful() {
    status_ = Status::Success;
}

void PaymentTransaction::markFailed() {
    status_ = Status::Failed;
}

void PaymentTransaction::markRefunded() {
    status_ = Status::Refunded;
}

bool PaymentTransaction::isCompleted() const {
    return status_ == Status::Success;
}

std::string PaymentTransaction::getStatusString() const {
    switch (status_) {
        case Status::Pending: return "Pending";
        case Status::Success: return "Success";
        case Status::Failed: return "Failed";
        case Status::Refunded: return "Refunded";
    }
    return "Unknown";
}

} // namespace online_learning
