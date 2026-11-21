#include "online_learning/progress/EngagementMetric.hpp"

namespace online_learning {

EngagementMetric::EngagementMetric()
    : metricId_(""), courseId_(""), viewsCount_(0), dropOffLessonId_(""), averageTimeSpentMinutes_(0.0) {}

EngagementMetric::EngagementMetric(std::string metricId, std::string courseId)
    : metricId_(std::move(metricId)), courseId_(std::move(courseId)),
      viewsCount_(0), dropOffLessonId_(""), averageTimeSpentMinutes_(0.0) {}

void EngagementMetric::recordPageView() {
    viewsCount_++;
}

void EngagementMetric::updateAverageTimeSpent(double sessionMinutes) {
    if (viewsCount_ <= 1) {
        averageTimeSpentMinutes_ = sessionMinutes;
    } else {
        averageTimeSpentMinutes_ = (averageTimeSpentMinutes_ * 0.8) + (sessionMinutes * 0.2);
    }
}

void EngagementMetric::setDropOffPoint(const std::string& lessonId) {
    dropOffLessonId_ = lessonId;
}

bool EngagementMetric::hasHighDropOffRate(int totalEnrollments) const {
    return totalEnrollments > 10 && viewsCount_ < (totalEnrollments / 2);
}

} // namespace online_learning
