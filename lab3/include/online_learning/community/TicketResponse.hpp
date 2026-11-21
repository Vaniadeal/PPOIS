#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class TicketResponse {
private:
    std::string responseId_;
    std::string ticketId_;
    std::string responderId_;
    std::string message_;
    int64_t timestamp_;

public:
    TicketResponse();
    TicketResponse(std::string responseId, std::string ticketId,
                   std::string responderId, std::string message, int64_t timestamp);

    void updateMessage(const std::string& newMessage);
    bool isFromAdmin(const std::string& adminId) const;
    std::string formatMessageBubble() const;

    const std::string& getResponseId() const { return responseId_; }
    const std::string& getTicketId() const { return ticketId_; }
    const std::string& getResponderId() const { return responderId_; }
    const std::string& getMessage() const { return message_; }
    int64_t getTimestamp() const { return timestamp_; }
};

} // namespace online_learning
