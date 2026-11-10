#include "online_learning/community/TicketResponse.hpp"

namespace online_learning {

TicketResponse::TicketResponse()
    : responseId_(""), ticketId_(""), responderId_(""), message_(""), timestamp_(0) {}

TicketResponse::TicketResponse(std::string responseId, std::string ticketId,
                               std::string responderId, std::string message, int64_t timestamp)
    : responseId_(std::move(responseId)), ticketId_(std::move(ticketId)),
      responderId_(std::move(responderId)), message_(std::move(message)), timestamp_(timestamp) {}

void TicketResponse::updateMessage(const std::string& newMessage) {
    message_ = newMessage;
}

bool TicketResponse::isFromAdmin(const std::string& adminId) const {
    return responderId_ == adminId;
}

std::string TicketResponse::formatMessageBubble() const {
    return "[" + responderId_ + "]: " + message_;
}

} // namespace online_learning
