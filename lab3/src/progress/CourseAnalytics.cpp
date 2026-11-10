#include "online_learning/progress/CourseAnalytics.hpp"

namespace online_learning {

CourseAnalytics::CourseAnalytics()
    : courseId_(""), enrolledStudentsCount_(0), averageRating_(5.0),
      completionRatePercent_(0.0), totalRevenue_(0.0), activeReviewsCount_(0) {}

CourseAnalytics::CourseAnalytics(std::string courseId)
    : courseId_(std::move(courseId)), enrolledStudentsCount_(0), averageRating_(5.0),
      completionRatePercent_(0.0), totalRevenue_(0.0), activeReviewsCount_(0) {}

void CourseAnalytics::incrementEnrollment() {
    enrolledStudentsCount_++;
}

void CourseAnalytics::addRevenue(double amount) {
    if (amount > 0.0) {
        totalRevenue_ += amount;
    }
}

void CourseAnalytics::recalculateCompletionRate(int completedCount) {
    if (enrolledStudentsCount_ > 0) {
        completionRatePercent_ = (static_cast<double>(completedCount) / enrolledStudentsCount_) * 100.0;
    }
}

void CourseAnalytics::updateRatingAverage(double newScore) {
    activeReviewsCount_++;
    averageRating_ = ((averageRating_ * (activeReviewsCount_ - 1)) + newScore) / activeReviewsCount_;
}

std::string CourseAnalytics::generateSummaryReport() const {
    return "Course " + courseId_ + " | Students: " + std::to_string(enrolledStudentsCount_) +
           " | Rating: " + std::to_string(averageRating_) +
           " | Revenue: $" + std::to_string(totalRevenue_);
}

} // namespace online_learning
