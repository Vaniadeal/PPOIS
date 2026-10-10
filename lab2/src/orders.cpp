#include "orders.hpp"
#include <algorithm>

namespace pharma {

OrderItem::OrderItem(std::string medId, std::string name, int qty, double price,
                     double discount)
    : medicineId_(std::move(medId)), medicineName_(std::move(name)),
      quantity_(qty), unitPrice_(price), discountPercent_(discount) {
    if (qty <= 0) throw InvalidOrderDataException("non-positive quantity");
}

double OrderItem::lineTotal() const {
    return quantity_ * unitPrice_ * (1.0 - discountPercent_ / 100.0);
}
void OrderItem::applyDiscount(double percent) {
    if (percent < 0 || percent > 100)
        throw InvalidOrderDataException("invalid discount");
    discountPercent_ = percent;
}
bool OrderItem::isValid() const {
    return quantity_ > 0 && unitPrice_ >= 0;
}

Payment::Payment(std::string id, std::string invoiceId, std::string customerId,
                 double amount, Date paidAt, std::string method)
    : id_(std::move(id)), invoiceId_(std::move(invoiceId)),
      customerId_(std::move(customerId)), amount_(amount), paidAt_(paidAt),
      method_(std::move(method)), completed_(false) {}

bool Payment::process(Customer& c) {
    try {
        c.withdraw(amount_);
        completed_ = true;
        return true;
    } catch (...) {
        throw PaymentFailedException("withdraw failed");
    }
}
bool Payment::refund() { completed_ = false; return true; }

Invoice::Invoice(std::string id, std::string orderId, Date issued, Date due,
                 double total)
    : id_(std::move(id)), orderId_(std::move(orderId)), issued_(issued),
      dueDate_(due), totalAmount_(total), paidAmount_(0.0), status_("open") {}

void Invoice::addPayment(double amount) {
    paidAmount_ += amount;
    if (paidAmount_ >= totalAmount_) status_ = "paid";
}
bool Invoice::isPaid() const { return paidAmount_ >= totalAmount_; }
double Invoice::remaining() const { return totalAmount_ - paidAmount_; }

Delivery::Delivery(std::string id, std::string shipmentId, std::string driverId,
                   std::string vehicleId, std::string routeId, Date planned)
    : id_(std::move(id)), shipmentId_(std::move(shipmentId)),
      driverId_(std::move(driverId)), vehicleId_(std::move(vehicleId)),
      routeId_(std::move(routeId)), plannedAt_(planned), status_("planned") {}

bool Delivery::assignDriver(const std::string& driverId) {
    if (driverId.empty()) throw DriverNotAvailableException(driverId);
    driverId_ = driverId;
    return true;
}
bool Delivery::start() { status_ = "in_progress"; return true; }
bool Delivery::complete() { status_ = "done"; return true; }
bool Delivery::isOnTime(const Date& now) const { return now <= plannedAt_; }

Shipment::Shipment(std::string id, std::string orderId, std::string warehouseId,
                   std::string waybillId)
    : id_(std::move(id)), orderId_(std::move(orderId)),
      warehouseId_(std::move(warehouseId)), waybillId_(std::move(waybillId)),
      shippedAt_(), status_("pending") {}

bool Shipment::markShipped() {
    shippedAt_ = std::chrono::system_clock::now();
    status_ = "in_transit";
    return true;
}
bool Shipment::markDelivered() { status_ = "delivered"; return true; }

Order::Order(std::string id, std::string customerId, Date created)
    : id_(std::move(id)), customerId_(std::move(customerId)), total_(0.0),
      status_("draft"), created_(created) {}

void Order::addItem(const OrderItem& item) {
    if (!item.isValid()) throw InvalidOrderDataException("invalid item");
    items_.push_back(item);
    total_ = calculateTotal();
}
void Order::removeItem(const std::string& medicineId) {
    items_.erase(std::remove_if(items_.begin(), items_.end(),
        [&](const OrderItem& i){ return i.medicineId() == medicineId; }),
        items_.end());
    total_ = calculateTotal();
}
double Order::calculateTotal() const {
    double sum = 0.0;
    for (const auto& i : items_) sum += i.lineTotal();
    return sum;
}
bool Order::submit() {
    if (items_.empty()) throw InvalidOrderDataException("empty order");
    status_ = "submitted";
    return true;
}
bool Order::cancel() { status_ = "cancelled"; return true; }
bool Order::confirmPayment(const Payment& p) {
    if (!p.isCompleted()) throw PaymentFailedException("not completed");
    status_ = "paid";
    return true;
}

Return::Return(std::string id, std::string orderId, std::string medId, int qty,
               std::string reason, Date at)
    : id_(std::move(id)), orderId_(std::move(orderId)),
      medicineId_(std::move(medId)), quantity_(qty), reason_(std::move(reason)),
      createdAt_(at), refundAmount_(0.0) {}

double Return::calculateRefund(double unitPrice) const {
    return quantity_ * unitPrice;
}
bool Return::approve() { return true; }
bool Return::reject(const std::string&) { return false; }

DeliverySchedule::DeliverySchedule(std::string v, std::string d, std::string o,
                                   Date dep, Date arr)
    : vehicleId_(std::move(v)), driverId_(std::move(d)), orderId_(std::move(o)),
      departure_(dep), arrival_(arr), status_("scheduled") {}

bool DeliverySchedule::isOverdue(const Date& now) const {
    return now > arrival_ && status_ != "done";
}
void DeliverySchedule::reschedule(Date newDep, Date newArr) {
    departure_ = newDep; arrival_ = newArr;
}
long long DeliverySchedule::durationMinutes() const {
    return std::chrono::duration_cast<std::chrono::minutes>(
        arrival_ - departure_).count();
}

} 