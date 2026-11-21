#include "online_learning/community/SupportTicket.hpp"

namespace online_learning {

SupportTicket::SupportTicket()
    : ticketId_(""), userId_(""), subject_(""), description_(""),
      priority_(Priority::Medium), status_(Status::Open), assignedAdminId_("") {}

SupportTicket::SupportTicket(std::string ticketId, std::string userId, std::string subject,
                             std::string desc, Priority priority)
    : ticketId_(std::move(ticketId)), userId_(std::move(userId)),
      subject_(std::move(subject)), description_(std::move(desc)),
      priority_(priority), status_(Status::Open), assignedAdminId_("") {}

void SupportTicket::assignToAdmin(const std::string& adminId) {
    assignedAdminId_ = adminId;
    status_ = Status::InProgress;
}

void SupportTicket::resolveTicket() {
    status_ = Status::Resolved;
}

void SupportTicket::addResponse(const TicketResponse& response) {
    responses_.push_back(response);
}

bool SupportTicket::isUrgent() const {
    return priority_ == Priority::High || priority_ == Priority::Critical;
}

std::string SupportTicket::getStatusString() const {
    switch (status_) {
        case Status::Open: return "Open";
        case Status::InProgress: return "InProgress";
        case Status::Resolved: return "Resolved";
        case Status::Closed: return "Closed";
    }
    return "Unknown";
}

} // namespace online_learning
