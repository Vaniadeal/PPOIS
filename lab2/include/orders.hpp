#pragma once
#include "core.hpp"
#include <vector>

namespace pharma {

class OrderItem {
    std::string medicineId_;
    std::string medicineName_;
    int quantity_;
    double unitPrice_;
    double discountPercent_;
public:
    OrderItem(std::string medId, std::string name, int qty, double price,
              double discount = 0.0);
    double lineTotal() const;
    void applyDiscount(double percent);
    bool isValid() const;
    const std::string& medicineId() const { return medicineId_; }
    int quantity() const { return quantity_; }
    double unitPrice() const { return unitPrice_; }
};

class Payment {
    std::string id_;
    std::string invoiceId_;
    std::string customerId_;
    double amount_;
    Date paidAt_;
    std::string method_;
    bool completed_;
public:
    Payment(std::string id, std::string invoiceId, std::string customerId,
            double amount, Date paidAt, std::string method);
    bool process(Customer& c);
    bool refund();
    bool isCompleted() const { return completed_; }
    double amount() const { return amount_; }
};

class Invoice {
    std::string id_;
    std::string orderId_;
    Date issued_;
    Date dueDate_;
    double totalAmount_;
    double paidAmount_;
    std::string status_;
public:
    Invoice(std::string id, std::string orderId, Date issued, Date due,
            double total);
    void addPayment(double amount);
    bool isPaid() const;
    double remaining() const;
};

class Delivery {
    std::string id_;
    std::string shipmentId_;
    std::string driverId_;
    std::string vehicleId_;
    std::string routeId_;
    Date plannedAt_;
    std::string status_;
public:
    Delivery(std::string id, std::string shipmentId, std::string driverId,
             std::string vehicleId, std::string routeId, Date planned);
    bool assignDriver(const std::string& driverId);
    bool start();
    bool complete();
    bool isOnTime(const Date& now) const;
};

class Shipment {
    std::string id_;
    std::string orderId_;
    std::string warehouseId_;
    std::string waybillId_;
    Date shippedAt_;
    std::string status_;
public:
    Shipment(std::string id, std::string orderId, std::string warehouseId,
             std::string waybillId);
    bool markShipped();
    bool markDelivered();
    bool isInTransit() const { return status_ == "in_transit"; }
};

class Order {
    std::string id_;
    std::string customerId_;
    std::vector<OrderItem> items_;
    double total_;
    std::string status_;
    Date created_;
    std::string paymentId_;
    std::string deliveryId_;
public:
    Order(std::string id, std::string customerId, Date created);

    void addItem(const OrderItem& item);
    void removeItem(const std::string& medicineId);
    double calculateTotal() const;
    bool submit();
    bool cancel();
    bool confirmPayment(const Payment& p);

    const std::string& id() const { return id_; }
    const std::string& status() const { return status_; }
    const std::vector<OrderItem>& items() const { return items_; }
    const std::string& customerId() const { return customerId_; }
};

class Return {
    std::string id_;
    std::string orderId_;
    std::string medicineId_;
    int quantity_;
    std::string reason_;
    Date createdAt_;
    double refundAmount_;
public:
    Return(std::string id, std::string orderId, std::string medId, int qty,
           std::string reason, Date at);
    double calculateRefund(double unitPrice) const;
    bool approve();
    bool reject(const std::string& why);
};

class DeliverySchedule {
    std::string vehicleId_;
    std::string driverId_;
    std::string orderId_;
    Date departure_;
    Date arrival_;
    std::string status_;
public:
    DeliverySchedule(std::string v, std::string d, std::string o,
                     Date dep, Date arr);
    bool isOverdue(const Date& now) const;
    void reschedule(Date newDep, Date newArr);
    long long durationMinutes() const;
};

} 