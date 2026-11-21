#include "online_learning/platform/Notification.hpp"

namespace online_learning {

Notification::Notification()
    : notificationId_(""), recipientId_(""), title_(""),
      message_(""), isRead_(false), timestamp_(0) {}

Notification::Notification(std::string id, std::string recipientId,
                           std::string title, std::string message, int64_t timestamp)
    : notificationId_(std::move(id)), recipientId_(std::move(recipientId)),
      title_(std::move(title)), message_(std::move(message)),
      isRead_(false), timestamp_(timestamp) {}

void Notification::markAsRead() {
    isRead_ = true;
}

bool Notification::isUnread() const {
    return !isRead_;
}

std::string Notification::formatNotificationBrief() const {
    std::string unreadTag = isRead_ ? "[Read]" : "[NEW]";
    return unreadTag + " " + title_ + ": " + message_;
}

} // namespace online_learning
