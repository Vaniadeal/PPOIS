#include "employees.hpp"

namespace pharma {

Employee::Employee(std::string id, std::string name, Address addr, ContactInfo c,
                   std::string position, Date hired, double salary,
                   std::string deptId)
    : id_(std::move(id)), fullName_(std::move(name)), address_(std::move(addr)),
      contact_(std::move(c)), position_(std::move(position)), hired_(hired),
      salary_(salary), departmentId_(std::move(deptId)) {}

int Employee::yearsOfService(const Date& now) const {
    auto secs = std::chrono::duration_cast<std::chrono::hours>(now - hired_).count();
    return static_cast<int>(secs / (24 * 365));
}
double Employee::calculateBonus() const { return salary_ * 0.05; }
bool Employee::isEligibleForRaise() const {
    return yearsOfService(std::chrono::system_clock::now()) >= 2;
}
void Employee::transferToDepartment(const std::string& deptId) {
    departmentId_ = deptId;
}

Manager::Manager(std::string id, std::string name, Address addr, ContactInfo c,
                 Date hired, double salary, std::string deptId, int subs)
    : Employee(std::move(id), std::move(name), std::move(addr), std::move(c),
               "Manager", hired, salary, std::move(deptId)), subordinates_(subs) {}

double Manager::calculateBonus() const {
    return salary() * (0.15 + 0.005 * subordinates_);
}
bool Manager::approveOrder(const std::string&) const { return true; }
void Manager::assignTask(const std::string&) const {}

Pharmacist::Pharmacist(std::string id, std::string name, Address addr,
                       ContactInfo c, Date hired, double salary,
                       std::string deptId, std::string license)
    : Employee(std::move(id), std::move(name), std::move(addr), std::move(c),
               "Pharmacist", hired, salary, std::move(deptId)),
      licenseNumber_(std::move(license)) {}

bool Pharmacist::dispense(const PrescriptionMedicine& m,
                          const std::string& rx) const {
    if (rx.empty()) throw PrescriptionRequiredException(m.name());
    return !m.isExpired();
}
bool Pharmacist::consult(const Customer&) const { return true; }
double Pharmacist::calculateBonus() const { return salary() * 0.08; }

WarehouseWorker::WarehouseWorker(std::string id, std::string name, Address addr,
                                 ContactInfo c, Date hired, double salary,
                                 std::string deptId)
    : Employee(std::move(id), std::move(name), std::move(addr), std::move(c),
               "WarehouseWorker", hired, salary, std::move(deptId)) {}

bool WarehouseWorker::receiveBatch(const Batch& b) { return !b.isExpired(); }
bool WarehouseWorker::shipOrder(const Order& o) { return o.status() != "draft"; }
double WarehouseWorker::calculateBonus() const { return salary() * 0.04; }

Driver::Driver(std::string id, std::string name, Address addr, ContactInfo c,
               Date hired, double salary, std::string deptId,
               std::string license, std::string vehicle)
    : Employee(std::move(id), std::move(name), std::move(addr), std::move(c),
               "Driver", hired, salary, std::move(deptId)),
      licenseNumber_(std::move(license)), vehicleId_(std::move(vehicle)),
      available_(true) {}

bool Driver::canDrive(const std::string& vehicleType) const {
    if (!available_) throw DriverNotAvailableException(fullName());
    return vehicleType == "truck" || vehicleType == "van" ||
           vehicleType == "refrigerated";
}
double Driver::calculateBonus() const { return salary() * 0.06; }

Accountant::Accountant(std::string id, std::string name, Address addr,
                       ContactInfo c, Date hired, double salary,
                       std::string deptId)
    : Employee(std::move(id), std::move(name), std::move(addr), std::move(c),
               "Accountant", hired, salary, std::move(deptId)) {}

bool Accountant::processPayment(const Payment& p) { return p.isCompleted(); }
bool Accountant::issueInvoice(const Invoice& i) { return i.remaining() > 0; }
double Accountant::calculateBonus() const { return salary() * 0.05; }

SalesRepresentative::SalesRepresentative(std::string id, std::string name,
                                         Address addr, ContactInfo c, Date hired,
                                         double salary, std::string deptId,
                                         double target)
    : Employee(std::move(id), std::move(name), std::move(addr), std::move(c),
               "SalesRepresentative", hired, salary, std::move(deptId)),
      target_(target), sold_(0.0) {}

bool SalesRepresentative::makeDeal(const Customer&, double amount) {
    if (amount <= 0) throw InvalidOrderDataException("non-positive amount");
    sold_ += amount; return true;
}
double SalesRepresentative::progressPercent() const {
    return target_ > 0 ? sold_ / target_ * 100.0 : 0.0;
}
double SalesRepresentative::calculateBonus() const {
    return salary() * (0.05 + progressPercent() / 1000.0);
}

QualityController::QualityController(std::string id, std::string name, Address addr,
                                     ContactInfo c, Date hired, double salary,
                                     std::string deptId)
    : Employee(std::move(id), std::move(name), std::move(addr), std::move(c),
               "QualityController", hired, salary, std::move(deptId)) {}

bool QualityController::inspectBatch(const Batch& b) const { return !b.isExpired(); }
bool QualityController::initiateRecall(const Medicine& m) const {
    if (m.isExpired()) throw RecallException(m.name());
    return true;
}
double QualityController::calculateBonus() const { return salary() * 0.07; }

LogisticsManager::LogisticsManager(std::string id, std::string name, Address addr,
                                   ContactInfo c, Date hired, double salary,
                                   std::string deptId)
    : Employee(std::move(id), std::move(name), std::move(addr), std::move(c),
               "LogisticsManager", hired, salary, std::move(deptId)) {}

bool LogisticsManager::optimizeRoute(const Route& r) const {
    return r.estimateFuel(10.0) > 0;
}
bool LogisticsManager::assignVehicle(const std::string&, const std::string&) const {
    return true;
}
double LogisticsManager::calculateBonus() const { return salary() * 0.09; }

} 