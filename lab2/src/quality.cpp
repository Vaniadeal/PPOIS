#include "quality.hpp"

namespace pharma {

Recall::Recall(std::string id, std::string medId, std::string batchId,
               std::string reason, Date at)
    : id_(std::move(id)), medicineId_(std::move(medId)),
      batchId_(std::move(batchId)), reason_(std::move(reason)),
      initiatedAt_(at), status_("active") {}

bool Recall::notifyCustomer(const std::string& custId) {
    customerId_ = custId; return true;
}
bool Recall::close() { status_ = "closed"; return true; }

AdverseEvent::AdverseEvent(std::string id, std::string medId, std::string custId,
                           std::string description, std::string severity, Date at)
    : id_(std::move(id)), medicineId_(std::move(medId)),
      customerId_(std::move(custId)), description_(std::move(description)),
      severity_(std::move(severity)), reportedAt_(at) {}

bool AdverseEvent::isSerious() const {
    return severity_ == "high" || severity_ == "critical";
}
bool AdverseEvent::reportToAuthority(const std::string&) {
    if (severity_.empty()) throw AdverseEventException("no severity");
    return true;
}
void AdverseEvent::attachEmployee(const std::string& empId) {
    employeeId_ = empId;
}

ClinicalTrial::ClinicalTrial(std::string id, std::string medId, Date start)
    : id_(std::move(id)), medicineId_(std::move(medId)), started_(start),
      participants_(0) {}

bool ClinicalTrial::addParticipant(const std::string& custId) {
    if (custId.empty()) return false;
    customerId_ = custId; ++participants_; return true;
}
double ClinicalTrial::successRate(int successful, int total) const {
    return total > 0 ? static_cast<double>(successful) / total * 100.0 : 0.0;
}
bool ClinicalTrial::conclude(const std::string& result) {
    result_ = result;
    ended_ = std::chrono::system_clock::now();
    return true;
}

} 