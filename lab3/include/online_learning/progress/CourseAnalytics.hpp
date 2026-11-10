#pragma once
#include <string>

namespace online_learning {

class CourseAnalytics {
private:
    std::string courseId_;
    int enrolledStudentsCount_;
    double averageRating_;
    double completionRatePercent_;
    double totalRevenue_;
    int activeReviewsCount_;

public:
    CourseAnalytics();
    explicit CourseAnalytics(std::string courseId);

    void incrementEnrollment();
    void addRevenue(double amount);
    void recalculateCompletionRate(int completedCount);
    void updateRatingAverage(double newScore);
    std::string generateSummaryReport() const;

    const std::string& getCourseId() const { return courseId_; }
    int getEnrolledStudentsCount() const { return enrolledStudentsCount_; }
    double getAverageRating() const { return averageRating_; }
    double getCompletionRatePercent() const { return completionRatePercent_; }
    double getTotalRevenue() const { return totalRevenue_; }
    int getActiveReviewsCount() const { return activeReviewsCount_; }
};

} // namespace online_learning
