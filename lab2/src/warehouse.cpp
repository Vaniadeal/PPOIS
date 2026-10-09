#include "warehouse.hpp"

namespace pharma {

Rack::Rack(std::string id, int capacity, std::string zoneId)
    : id_(std::move(id)), capacity_(capacity), used_(0),
      zoneId_(std::move(zoneId)) {}

bool Rack::placeBatch(const std::string& batchId, int qty) {
    if (qty + used_ > capacity_) throw WarehouseFullException(zoneId_);
    batchId_ = batchId; used_ += qty; return true;
}
bool Rack::removeBatch() { used_ = 0; batchId_.clear(); return true; }

StorageZone::StorageZone(std::string id, std::string name, std::string whId,
                         double minT, double maxT, int cap)
    : id_(std::move(id)), name_(std::move(name)), warehouseId_(std::move(whId)),
      minTemp_(minT), maxTemp_(maxT), capacity_(cap) {}

bool StorageZone::isTemperatureValid(double t) const {
    return t >= minTemp_ && t <= maxTemp_;
}
bool StorageZone::canStore(int qty) const { return qty <= capacity_; }

Batch::Batch(std::string id, std::string medId, std::string supId, int qty,
             Date prod, Date exp, double price)
    : id_(std::move(id)), medicineId_(std::move(medId)),
      supplierId_(std::move(supId)), quantity_(qty), production_(prod),
      expiry_(exp), purchasePrice_(price) {}

bool Batch::isExpired() const { return pharma::isExpired(expiry_); }
double Batch::totalValue() const { return quantity_ * purchasePrice_; }
bool Batch::assignRack(const std::string& rackId) { rackId_ = rackId; return true; }

BatchItem::BatchItem(std::string id, std::string batchId, std::string barcode,
                     int qty)
    : id_(std::move(id)), batchId_(std::move(batchId)),
      barcode_(std::move(barcode)), quantity_(qty), status_("quarantine") {}

bool BatchItem::quarantine() { status_ = "quarantine"; return true; }
bool BatchItem::release() { status_ = "available"; return true; }

Warehouse::Warehouse(std::string id, std::string name, Address addr, int capacity)
    : id_(std::move(id)), name_(std::move(name)), address_(std::move(addr)),
      capacity_(capacity), used_(0) {}

void Warehouse::addZone(const std::string& zoneId) { zoneIds_.push_back(zoneId); }
bool Warehouse::canAccept(int qty) const { return used_ + qty <= capacity_; }
double Warehouse::utilization() const {
    return capacity_ > 0 ? static_cast<double>(used_) / capacity_ * 100.0 : 0.0;
}
bool Warehouse::requiresRefrigeration() const { return !zoneIds_.empty(); }

Inventory::Inventory(std::string id, std::string whId, std::string empId, Date at)
    : id_(std::move(id)), warehouseId_(std::move(whId)),
      employeeId_(std::move(empId)), checkedAt_(at), result_("pending") {}

void Inventory::addBatch(const std::string& batchId) { batchIds_.push_back(batchId); }
bool Inventory::performCheck() { result_ = "ok"; return true; }
double Inventory::discrepancyPercent() const { return 0.0; }
bool Inventory::hasDiscrepancies() const { return result_ == "discrepancy"; }

} 