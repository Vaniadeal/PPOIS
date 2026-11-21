#include "online_learning/platform/NotificationService.hpp"

namespace online_learning {

NotificationService::NotificationService() : totalSentCount_(0) {}

void NotificationService::sendNotification(const Notification& notification) {
    notifications_.push_back(notification);
    totalSentCount_++;
}

void NotificationService::markNotificationAsRead(const std::string& notificationId) {
    for (auto& notif : notifications_) {
        if (notif.getNotificationId() == notificationId) {
            notif.markAsRead();
            break;
        }
    }
}

int NotificationService::getUnreadCountForRecipient(const std::string& recipientId) const {
    int count = 0;
    for (const auto& notif : notifications_) {
        if (notif.getRecipientId() == recipientId && notif.isUnread()) {
            count++;
        }
    }
    return count;
}

std::vector<Notification> NotificationService::getNotificationsForRecipient(const std::string& recipientId) const {
    std::vector<Notification> result;
    for (const auto& notif : notifications_) {
        if (notif.getRecipientId() == recipientId) {
            result.push_back(notif);
        }
    }
    return result;
}

} // namespace online_learning
