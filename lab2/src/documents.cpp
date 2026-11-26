#include "documents.hpp"

namespace pharma {

Document::Document(std::string id, std::string title, Date at, std::string author)
    : id_(std::move(id)), title_(std::move(title)), createdAt_(at),
      authorId_(std::move(author)), signed_(false) {}

bool Document::sign(const std::string&) { signed_ = true; return true; }
std::string Document::render() const { return title_; }

Waybill::Waybill(std::string id, Date at, std::string author,
                 std::string shipmentId, std::string driverId,
                 std::string vehicleId, std::string route)
    : Document(std::move(id), "Waybill", at, std::move(author)),
      shipmentId_(std::move(shipmentId)), driverId_(std::move(driverId)),
      vehicleId_(std::move(vehicleId)), routeInfo_(std::move(route)) {}

std::string Waybill::render() const { return "Waybill " + id_; }
bool Waybill::validate() const {
    return !shipmentId_.empty() && !driverId_.empty() && !vehicleId_.empty();
}
bool Waybill::matchesShipment(const std::string& s) const {
    return shipmentId_ == s;
}

Contract::Contract(std::string id, std::string supId, std::string custId,
                   std::string manId, Date signedAt, Date until, double amount)
    : id_(std::move(id)), supplierId_(std::move(supId)),
      customerId_(std::move(custId)), manufacturerId_(std::move(manId)),
      signed_(signedAt), validUntil_(until), amount_(amount) {}

bool Contract::isActive(const Date& now) const { return now <= validUntil_; }
bool Contract::renew(int extraMonths) {
    constexpr int HOURS_PER_MONTH = 24 * 30;
    validUntil_ += std::chrono::hours(HOURS_PER_MONTH * extraMonths);
    return true;
}
double Contract::calculatePenalty(int daysLate) const {
    constexpr double PENALTY_RATE = 0.001;
    return amount_ * PENALTY_RATE * daysLate;
}
bool Contract::covers(const std::string&) const { return true; }

PriceList::PriceList(std::string id, Date from, std::string currency)
    : id_(std::move(id)), validFrom_(from), currency_(std::move(currency)) {}

void PriceList::setPrice(const std::string& medId, double price) {
    prices_.emplace_back(medId, price);
}
std::optional<double> PriceList::getPrice(const std::string& medId) const {
    for (const auto& p : prices_) if (p.first == medId) return p.second;
    return std::nullopt;
}
double PriceList::indexation(double percent) const {
    double sum = 0.0;
    for (const auto& p : prices_) sum += p.second * (1.0 + percent / 100.0);
    return sum;
}

Promotion::Promotion(std::string id, std::string medId, Date s, Date e,
                     double pct, std::string desc)
    : id_(std::move(id)), medicineId_(std::move(medId)), start_(s), end_(e),
      percent_(pct), description_(std::move(desc)) {}

bool Promotion::isActive(const Date& now) const {
    return now >= start_ && now <= end_;
}
double Promotion::apply(double price) const {
    return price * (1.0 - percent_/100.0);
}
bool Promotion::targetsOrder(const Order& o) const {
    for (const auto& i : o.items())
        if (i.medicineId() == medicineId_) return true;
    return false;
}

Discount::Discount(std::string id, std::string custId, std::string orderId,
                   double percent, std::string reason)
    : id_(std::move(id)), customerId_(std::move(custId)),
      orderId_(std::move(orderId)), percent_(percent), reason_(std::move(reason)) {}

double Discount::apply(double total) const {
    return total * (1.0 - percent_/100.0);
}
bool Discount::isStackable() const { return percent_ < 20.0; }

PromotionCode::PromotionCode(std::string code, std::string discId, Date until,
                             int maxUses)
    : code_(std::move(code)), discountId_(std::move(discId)), validUntil_(until),
      maxUses_(maxUses), used_(0) {}

bool PromotionCode::redeem() {
    if (!isValid()) return false;
    ++used_; return true;
}
bool PromotionCode::isValid() const {
    return used_ < maxUses_ && validUntil_ > std::chrono::system_clock::now();
}

}