#include "reports.hpp"
#include <fstream>

namespace pharma {

Report::Report(std::string id, std::string title, Date at, std::string author)
    : id_(std::move(id)), title_(std::move(title)), generatedAt_(at),
      authorId_(std::move(author)) {}

std::string Report::render() const { return title_; }
bool Report::exportTo(const std::string& path) const {
    std::ofstream f(path);
    if (!f) return false;
    f << render();
    return true;
}

SalesReport::SalesReport(std::string id, Date at, std::string author,
                         std::string orderId, std::string medId, double revenue)
    : Report(std::move(id), "SalesReport", at, std::move(author)),
      orderId_(std::move(orderId)), medicineId_(std::move(medId)),
      revenue_(revenue) {}

double SalesReport::summarize() const { return revenue_; }
double SalesReport::averageOrderValue(int orders) const {
    return orders > 0 ? revenue_ / orders : 0.0;
}
std::string SalesReport::render() const {
    return "Sales " + std::to_string(revenue_);
}

StockReport::StockReport(std::string id, Date at, std::string author,
                         std::string whId, std::string batchId, int totalUnits)
    : Report(std::move(id), "StockReport", at, std::move(author)),
      warehouseId_(std::move(whId)), batchId_(std::move(batchId)),
      totalUnits_(totalUnits) {}

double StockReport::summarize() const { return static_cast<double>(totalUnits_); }
bool StockReport::isLowStock(int threshold) const { return totalUnits_ < threshold; }
std::string StockReport::render() const {
    return "Stock " + std::to_string(totalUnits_);
}

FinancialReport::FinancialReport(std::string id, Date at, std::string author,
                                 std::string payId, std::string invId, double net)
    : Report(std::move(id), "FinancialReport", at, std::move(author)),
      paymentId_(std::move(payId)), invoiceId_(std::move(invId)),
      netAmount_(net) {}

double FinancialReport::summarize() const { return netAmount_; }
double FinancialReport::tax(double rate) const {
    return netAmount_ * rate / 100.0;
}
std::string FinancialReport::render() const {
    return "Financial net=" + std::to_string(netAmount_);
}

}