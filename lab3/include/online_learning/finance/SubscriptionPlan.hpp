#pragma once
#include <string>

namespace online_learning {

class SubscriptionPlan {
private:
    std::string planId_;
    std::string name_;
    double monthlyPrice_;
    int maxCoursesAccess_;
    int tierLevel_;
    bool isAvailable_;

public:
    SubscriptionPlan();
    SubscriptionPlan(std::string planId, std::string name, double price,
                     int maxCourses, int tierLevel);

    void updatePrice(double newPrice);
    void toggleAvailability(bool available);
    bool permitsCourseCount(int requestedCount) const;
    std::string getTierDescription() const;

    const std::string& getPlanId() const { return planId_; }
    const std::string& getName() const { return name_; }
    double getMonthlyPrice() const { return monthlyPrice_; }
    int getMaxCoursesAccess() const { return maxCoursesAccess_; }
    int getTierLevel() const { return tierLevel_; }
    bool isAvailable() const { return isAvailable_; }
};

} // namespace online_learning
