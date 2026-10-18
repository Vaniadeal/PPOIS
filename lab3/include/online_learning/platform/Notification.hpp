#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class Notification {
private:
    std::string notificationId_;
    std::string recipientId_;
    std::string title_;
    std::string message_;
    bool isRead_;
    int64_t timestamp_;

public:
    Notification();
    Notification(std::string id, std::string recipientId,
                 std::string title, std::string message, int64_t timestamp);

    void markAsRead();
    bool isUnread() const;
    std::string formatNotificationBrief() const;

    const std::string& getNotificationId() const { return notificationId_; }
    const std::string& getRecipientId() const { return recipientId_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getMessage() const { return message_; }
    int64_t getTimestamp() const { return timestamp_; }
};

} // namespace online_learning
