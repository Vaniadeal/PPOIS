#pragma once
#include "core.hpp"
#include "orders.hpp"

namespace pharma {

class Document {
protected:
    std::string id_;
    std::string title_;
    Date createdAt_;
    std::string authorId_;
    std::string content_;
    bool signed_;
public:
    Document(std::string id, std::string title, Date at, std::string author);
    virtual ~Document() = default;
    bool sign(const std::string& signer);
    bool isSigned() const { return signed_; }
    virtual std::string render() const;
    const std::string& id() const { return id_; }
};

class Waybill : public Document {
    std::string shipmentId_;
    std::string driverId_;
    std::string vehicleId_;
    std::string routeInfo_;
public:
    Waybill(std::string id, Date at, std::string author, std::string shipmentId,
            std::string driverId, std::string vehicleId, std::string route);
    std::string render() const override;
    bool validate() const;
    bool matchesShipment(const std::string& shipmentId) const;
};

class Contract {
    std::string id_;
    std::string supplierId_;
    std::string customerId_;
    std::string manufacturerId_;
    Date signed_;
    Date validUntil_;
    double amount_;
public:
    Contract(std::string id, std::string supId, std::string custId,
             std::string manId, Date signedAt, Date until, double amount);
    bool isActive(const Date& now) const;
    bool renew(int extraMonths);
    double calculatePenalty(int daysLate) const;
    bool covers(const std::string& medicineId) const;
};

class PriceList {
    std::string id_;
    Date validFrom_;
    std::vector<std::pair<std::string, double>> prices_;
    std::string currency_;
public:
    PriceList(std::string id, Date from, std::string currency);
    void setPrice(const std::string& medId, double price);
    std::optional<double> getPrice(const std::string& medId) const;
    double indexation(double percent) const;
};

class Promotion {
    std::string id_;
    std::string medicineId_;
    Date start_;
    Date end_;
    double percent_;
    std::string description_;
public:
    Promotion(std::string id, std::string medId, Date s, Date e, double pct,
              std::string desc);
    bool isActive(const Date& now) const;
    double apply(double price) const;
    bool targetsOrder(const Order& o) const;
};

class Discount {
    std::string id_;
    std::string customerId_;
    std::string orderId_;
    double percent_;
    std::string reason_;
public:
    Discount(std::string id, std::string custId, std::string orderId,
             double percent, std::string reason);
    double apply(double total) const;
    bool isStackable() const;
};

class PromotionCode {
    std::string code_;
    std::string discountId_;
    Date validUntil_;
    int maxUses_;
    int used_;
public:
    PromotionCode(std::string code, std::string discId, Date until, int maxUses);
    bool redeem();
    bool isValid() const;
    int remaining() const { return maxUses_ - used_; }
};

} 