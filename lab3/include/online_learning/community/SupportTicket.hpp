#pragma once
#include <string>
#include <vector>
#include "online_learning/community/TicketResponse.hpp"

namespace online_learning {

class SupportTicket {
public:
    enum class Priority { Low, Medium, High, Critical };
    enum class Status { Open, InProgress, Resolved, Closed };

private:
    std::string ticketId_;
    std::string userId_;
    std::string subject_;
    std::string description_;
    Priority priority_;
    Status status_;
    std::string assignedAdminId_;
    std::vector<TicketResponse> responses_;

public:
    SupportTicket();
    SupportTicket(std::string ticketId, std::string userId, std::string subject,
                  std::string desc, Priority priority);

    void assignToAdmin(const std::string& adminId);
    void resolveTicket();
    void addResponse(const TicketResponse& response);
    bool isUrgent() const;
    std::string getStatusString() const;

    const std::string& getTicketId() const { return ticketId_; }
    const std::string& getUserId() const { return userId_; }
    const std::string& getSubject() const { return subject_; }
    const std::string& getDescription() const { return description_; }
    Priority getPriority() const { return priority_; }
    Status getStatus() const { return status_; }
    const std::string& getAssignedAdminId() const { return assignedAdminId_; }
    const std::vector<TicketResponse>& getResponses() const { return responses_; }
};

} // namespace online_learning
