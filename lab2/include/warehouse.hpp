#pragma once
#include "core.hpp"

namespace pharma {

class Rack {
    std::string id_;
    int capacity_;
    int used_;
    std::string zoneId_;
    std::string batchId_;
public:
    Rack(std::string id, int capacity, std::string zoneId);
    bool placeBatch(const std::string& batchId, int qty);
    bool removeBatch();
    int freeSpace() const { return capacity_ - used_; }
};

class StorageZone {
    std::string id_;
    std::string name_;
    std::string warehouseId_;
    double minTemp_;
    double maxTemp_;
    int capacity_;
public:
    StorageZone(std::string id, std::string name, std::string whId,
                double minT, double maxT, int cap);
    bool isTemperatureValid(double t) const;
    bool canStore(int qty) const;
    bool requiresRefrigeration() const { return minTemp_ < 8.0; }
};

class Batch {
    std::string id_;
    std::string medicineId_;
    std::string supplierId_;
    int quantity_;
    Date production_;
    Date expiry_;
    std::string rackId_;
    double purchasePrice_;
public:
    Batch(std::string id, std::string medId, std::string supId, int qty,
          Date prod, Date exp, double price);
    bool isExpired() const;
    double totalValue() const;
    bool assignRack(const std::string& rackId);
    const std::string& id() const { return id_; }
    const std::string& medicineId() const { return medicineId_; }
    int quantity() const { return quantity_; }
    void setQuantity(int q) { quantity_ = q; }
};

class BatchItem {
    std::string id_;
    std::string batchId_;
    std::string barcode_;
    int quantity_;
    std::string status_;
public:
    BatchItem(std::string id, std::string batchId, std::string barcode, int qty);
    bool quarantine();
    bool release();
    bool isAvailable() const { return status_ == "available"; }
};

class Warehouse {
    std::string id_;
    std::string name_;
    Address address_;
    std::vector<std::string> zoneIds_;
    int capacity_;
    int used_;
    std::string managerId_;
public:
    Warehouse(std::string id, std::string name, Address addr, int capacity);
    void addZone(const std::string& zoneId);
    bool canAccept(int qty) const;
    double utilization() const;
    bool requiresRefrigeration() const;
    const std::string& id() const { return id_; }
};

class Inventory {
    std::string id_;
    std::string warehouseId_;
    std::string employeeId_;
    Date checkedAt_;
    std::vector<std::string> batchIds_;
    std::string result_;
public:
    Inventory(std::string id, std::string whId, std::string empId, Date at);
    void addBatch(const std::string& batchId);
    bool performCheck();
    double discrepancyPercent() const;
    bool hasDiscrepancies() const;
};

}   