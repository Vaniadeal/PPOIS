#pragma once
#include <string>

namespace online_learning {

class EngagementMetric {
private:
    std::string metricId_;
    std::string courseId_;
    int viewsCount_;
    std::string dropOffLessonId_;
    double averageTimeSpentMinutes_;

public:
    EngagementMetric();
    EngagementMetric(std::string metricId, std::string courseId);

    void recordPageView();
    void updateAverageTimeSpent(double sessionMinutes);
    void setDropOffPoint(const std::string& lessonId);
    bool hasHighDropOffRate(int totalEnrollments) const;

    const std::string& getMetricId() const { return metricId_; }
    const std::string& getCourseId() const { return courseId_; }
    int getViewsCount() const { return viewsCount_; }
    const std::string& getDropOffLessonId() const { return dropOffLessonId_; }
    double getAverageTimeSpentMinutes() const { return averageTimeSpentMinutes_; }
};

} // namespace online_learning
