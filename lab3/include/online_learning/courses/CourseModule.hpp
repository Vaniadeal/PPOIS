#pragma once
#include <string>
#include <vector>

namespace online_learning {

class CourseModule {
private:
    std::string moduleId_;
    std::string courseId_;
    std::string title_;
    int orderIndex_;
    bool isMandatory_;
    double passingScorePercentage_;
    std::vector<std::string> lessonIds_;

public:
    CourseModule();
    CourseModule(std::string moduleId, std::string courseId, std::string title,
                 int orderIndex, bool isMandatory, double passingScore);

    void addLessonId(const std::string& lessonId);
    void removeLessonId(const std::string& lessonId);
    void updatePassingScore(double newPassingScore);
    int getTotalLessonsCount() const;
    bool isEligibleForCompletion(double studentScore) const;

    const std::string& getModuleId() const { return moduleId_; }
    const std::string& getCourseId() const { return courseId_; }
    const std::string& getTitle() const { return title_; }
    int getOrderIndex() const { return orderIndex_; }
    bool isMandatory() const { return isMandatory_; }
    double getPassingScorePercentage() const { return passingScorePercentage_; }
    const std::vector<std::string>& getLessonIds() const { return lessonIds_; }
};

} // namespace online_learning
