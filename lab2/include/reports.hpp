#pragma once
#include "core.hpp"

namespace pharma {

class Report {
protected:
    std::string id_;
    std::string title_;
    Date generatedAt_;
    std::string authorId_;
    std::string content_;
public:
    Report(std::string id, std::string title, Date at, std::string author);
    virtual ~Report() = default;
    virtual std::string render() const;
    virtual double summarize() const = 0;
    bool exportTo(const std::string& path) const;
    const std::string& id() const { return id_; }
};

class SalesReport : public Report {
    std::string orderId_;
    std::string medicineId_;
    double revenue_;
public:
    SalesReport(std::string id, Date at, std::string author,
                std::string orderId, std::string medId, double revenue);
    double summarize() const override;
    double averageOrderValue(int orders) const;
    std::string render() const override;
};

class StockReport : public Report {
    std::string warehouseId_;
    std::string batchId_;
    int totalUnits_;
public:
    StockReport(std::string id, Date at, std::string author,
                std::string whId, std::string batchId, int totalUnits);
    double summarize() const override;
    bool isLowStock(int threshold) const;
    std::string render() const override;
};

class FinancialReport : public Report {
    std::string paymentId_;
    std::string invoiceId_;
    double netAmount_;
public:
    FinancialReport(std::string id, Date at, std::string author,
                    std::string payId, std::string invId, double net);
    double summarize() const override;
    double tax(double rate) const;
    std::string render() const override;
};

} 