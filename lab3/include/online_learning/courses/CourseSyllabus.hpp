#pragma once
#include <string>
#include <vector>

namespace online_learning {

class CourseSyllabus {
private:
    std::string courseId_;
    int totalDurationHours_;
    std::vector<std::string> learningObjectives_;
    std::vector<std::string> prerequisites_;

public:
    CourseSyllabus();
    CourseSyllabus(std::string courseId, int totalDurationHours);

    void addObjective(const std::string& objective);
    void addPrerequisite(const std::string& prerequisite);
    bool meetsPrerequisites(const std::vector<std::string>& studentSkills) const;
    int getObjectivesCount() const;

    const std::string& getCourseId() const { return courseId_; }
    int getTotalDurationHours() const { return totalDurationHours_; }
    const std::vector<std::string>& getLearningObjectives() const { return learningObjectives_; }
    const std::vector<std::string>& getPrerequisites() const { return prerequisites_; }
};

} // namespace online_learning
