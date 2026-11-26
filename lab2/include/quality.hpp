#pragma once
#include "core.hpp"

namespace pharma {

class Recall {
    std::string id_;
    std::string medicineId_;
    std::string batchId_;
    std::string customerId_;
    std::string reason_;
    Date initiatedAt_;
    std::string status_;
public:
    Recall(std::string id, std::string medId, std::string batchId,
           std::string reason, Date at);
    bool notifyCustomer(const std::string& custId);
    bool close();
    bool isActive() const { return status_ == "active"; }
};

class AdverseEvent {
    std::string id_;
    std::string medicineId_;
    std::string customerId_;
    std::string employeeId_;
    std::string description_;
    std::string severity_;
    Date reportedAt_;
public:
    AdverseEvent(std::string id, std::string medId, std::string custId,
                 std::string description, std::string severity, Date at);
    bool isSerious() const;
    bool reportToAuthority(const std::string& authority);
    void attachEmployee(const std::string& empId);
};

class ClinicalTrial {
    std::string id_;
    std::string medicineId_;
    std::string customerId_;
    Date started_;
    Date ended_;
    int participants_;
    std::string result_;
public:
    ClinicalTrial(std::string id, std::string medId, Date start);
    bool addParticipant(const std::string& custId);
    double successRate(int successful, int total) const;
    bool conclude(const std::string& result);
};

} 