#pragma once
#include <vector>
#include <string>
#include "online_learning/platform/Notification.hpp"

namespace online_learning {

class NotificationService {
private:
    std::vector<Notification> notifications_;
    int totalSentCount_;

public:
    NotificationService();

    void sendNotification(const Notification& notification);
    void markNotificationAsRead(const std::string& notificationId);
    int getUnreadCountForRecipient(const std::string& recipientId) const;
    std::vector<Notification> getNotificationsForRecipient(const std::string& recipientId) const;

    int getTotalSentCount() const { return totalSentCount_; }
};

} // namespace online_learning
