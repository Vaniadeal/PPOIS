#include "online_learning/finance/SubscriptionPlan.hpp"

namespace online_learning {

SubscriptionPlan::SubscriptionPlan()
    : planId_(""), name_(""), monthlyPrice_(0.0),
      maxCoursesAccess_(0), tierLevel_(0), isAvailable_(false) {}

SubscriptionPlan::SubscriptionPlan(std::string planId, std::string name, double price,
                                   int maxCourses, int tierLevel)
    : planId_(std::move(planId)), name_(std::move(name)), monthlyPrice_(price),
      maxCoursesAccess_(maxCourses), tierLevel_(tierLevel), isAvailable_(true) {}

void SubscriptionPlan::updatePrice(double newPrice) {
    if (newPrice >= 0.0) {
        monthlyPrice_ = newPrice;
    }
}

void SubscriptionPlan::toggleAvailability(bool available) {
    isAvailable_ = available;
}

bool SubscriptionPlan::permitsCourseCount(int requestedCount) const {
    return requestedCount <= maxCoursesAccess_;
}

std::string SubscriptionPlan::getTierDescription() const {
    return name_ + " Tier " + std::to_string(tierLevel_) + " ($" + std::to_string(monthlyPrice_) + "/mo)";
}

} // namespace online_learning
