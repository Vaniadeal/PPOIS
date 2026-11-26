#pragma once
#include "core.hpp"
#include "orders.hpp"
#include "warehouse.hpp"
#include "transport.hpp"
#include "quality.hpp"

namespace pharma {

class Employee {
protected:
    std::string id_;
    std::string fullName_;
    Address address_;
    ContactInfo contact_;
    std::string position_;
    Date hired_;
    double salary_;
    std::string departmentId_;
public:
    Employee(std::string id, std::string name, Address addr, ContactInfo c,
             std::string position, Date hired, double salary,
             std::string deptId);
    virtual ~Employee() = default;

    int yearsOfService(const Date& now) const;
    virtual double calculateBonus() const;
    bool isEligibleForRaise() const;
    void transferToDepartment(const std::string& deptId);

    const std::string& id() const { return id_; }
    const std::string& fullName() const { return fullName_; }
    double salary() const { return salary_; }
};

class Manager : public Employee {
    int subordinates_;
public:
    Manager(std::string id, std::string name, Address addr, ContactInfo c,
            Date hired, double salary, std::string deptId, int subs);
    double calculateBonus() const override;
    bool approveOrder(const std::string& orderId) const;
    void assignTask(const std::string& employeeId) const;
};

class Pharmacist : public Employee {
    std::string licenseNumber_;
public:
    Pharmacist(std::string id, std::string name, Address addr, ContactInfo c,
               Date hired, double salary, std::string deptId,
               std::string license);
    bool dispense(const PrescriptionMedicine& m, const std::string& rxId) const;
    bool consult(const Customer& c) const;
    double calculateBonus() const override;
};

class WarehouseWorker : public Employee {
public:
    WarehouseWorker(std::string id, std::string name, Address addr,
                    ContactInfo c, Date hired, double salary,
                    std::string deptId);
    bool receiveBatch(const Batch& b);
    bool shipOrder(const Order& o);
    double calculateBonus() const override;
};

class Driver : public Employee {
    std::string licenseNumber_;
    std::string vehicleId_;
    bool available_;
public:
    Driver(std::string id, std::string name, Address addr, ContactInfo c,
           Date hired, double salary, std::string deptId,
           std::string license, std::string vehicle);
    bool isAvailable() const { return available_; }
    void setAvailable(bool v) { available_ = v; }
    bool canDrive(const std::string& vehicleType) const;
    double calculateBonus() const override;
};

class Accountant : public Employee {
public:
    Accountant(std::string id, std::string name, Address addr, ContactInfo c,
               Date hired, double salary, std::string deptId);
    bool processPayment(const Payment& p);
    bool issueInvoice(const Invoice& i);
    double calculateBonus() const override;
};

class SalesRepresentative : public Employee {
    double target_;
    double sold_;
public:
    SalesRepresentative(std::string id, std::string name, Address addr,
                        ContactInfo c, Date hired, double salary,
                        std::string deptId, double target);
    bool makeDeal(const Customer& c, double amount);
    double progressPercent() const;
    double calculateBonus() const override;
};

class QualityController : public Employee {
public:
    QualityController(std::string id, std::string name, Address addr,
                      ContactInfo c, Date hired, double salary,
                      std::string deptId);
    bool inspectBatch(const Batch& b) const;
    bool initiateRecall(const Medicine& m) const;
    double calculateBonus() const override;
};

class LogisticsManager : public Employee {
public:
    LogisticsManager(std::string id, std::string name, Address addr,
                     ContactInfo c, Date hired, double salary,
                     std::string deptId);
    bool optimizeRoute(const Route& r) const;
    bool assignVehicle(const std::string& orderId,
                       const std::string& vehicleId) const;
    double calculateBonus() const override;
};

}