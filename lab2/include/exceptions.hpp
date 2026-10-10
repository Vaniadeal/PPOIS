#pragma once
#include <stdexcept>
#include <string>

namespace pharma {

class PharmaException : public std::runtime_error {
public:
    explicit PharmaException(const std::string& msg) : std::runtime_error(msg) {}
};

class MedicineNotFoundException : public PharmaException {
public:
    explicit MedicineNotFoundException(const std::string& id)
        : PharmaException("Medicine not found: " + id) {}
};

class InvalidMedicineDataException : public PharmaException {
public:
    explicit InvalidMedicineDataException(const std::string& why)
        : PharmaException("Invalid medicine data: " + why) {}
};

class PrescriptionRequiredException : public PharmaException {
public:
    explicit PrescriptionRequiredException(const std::string& med)
        : PharmaException("Prescription required for: " + med) {}
};

class ExpiredMedicineException : public PharmaException {
public:
    explicit ExpiredMedicineException(const std::string& med)
        : PharmaException("Medicine expired: " + med) {}
};

class InsufficientStockException : public PharmaException {
public:
    explicit InsufficientStockException(const std::string& med)
        : PharmaException("Insufficient stock for: " + med) {}
};

class OrderNotFoundException : public PharmaException {
public:
    explicit OrderNotFoundException(const std::string& id)
        : PharmaException("Order not found: " + id) {}
};

class InvalidOrderDataException : public PharmaException {
public:
    explicit InvalidOrderDataException(const std::string& why)
        : PharmaException("Invalid order data: " + why) {}
};

class PaymentFailedException : public PharmaException {
public:
    explicit PaymentFailedException(const std::string& why)
        : PharmaException("Payment failed: " + why) {}
};

class InsufficientFundsException : public PharmaException {
public:
    InsufficientFundsException(double need, double have)
        : PharmaException("Insufficient funds: need=" + std::to_string(need) +
                          " have=" + std::to_string(have)) {}
};

class DeliveryFailedException : public PharmaException {
public:
    explicit DeliveryFailedException(const std::string& why)
        : PharmaException("Delivery failed: " + why) {}
};

class VehicleNotAvailableException : public PharmaException {
public:
    explicit VehicleNotAvailableException(const std::string& plate)
        : PharmaException("Vehicle not available: " + plate) {}
};

class DriverNotAvailableException : public PharmaException {
public:
    explicit DriverNotAvailableException(const std::string& name)
        : PharmaException("Driver not available: " + name) {}
};

class WarehouseFullException : public PharmaException {
public:
    explicit WarehouseFullException(const std::string& wh)
        : PharmaException("Warehouse is full: " + wh) {}
};

class InvalidLicenseException : public PharmaException {
public:
    explicit InvalidLicenseException(const std::string& num)
        : PharmaException("Invalid license: " + num) {}
};

class CustomerNotFoundException : public PharmaException {
public:
    explicit CustomerNotFoundException(const std::string& id)
        : PharmaException("Customer not found: " + id) {}
};

class DuplicateCustomerException : public PharmaException {
public:
    explicit DuplicateCustomerException(const std::string& id)
        : PharmaException("Duplicate customer: " + id) {}
};

class ColdChainViolationException : public PharmaException {
public:
    ColdChainViolationException(double t, double minT, double maxT)
        : PharmaException("Cold chain violation: t=" + std::to_string(t) +
                          " range=[" + std::to_string(minT) + "," +
                          std::to_string(maxT) + "]") {}
};

class RecallException : public PharmaException {
public:
    explicit RecallException(const std::string& med)
        : PharmaException("Recall error for medicine: " + med) {}
};

class AdverseEventException : public PharmaException {
public:
    explicit AdverseEventException(const std::string& why)
        : PharmaException("Adverse event: " + why) {}
};

} 