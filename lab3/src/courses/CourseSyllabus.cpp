#include "online_learning/courses/CourseSyllabus.hpp"
#include <algorithm>

namespace online_learning {

CourseSyllabus::CourseSyllabus() : courseId_(""), totalDurationHours_(0) {}

CourseSyllabus::CourseSyllabus(std::string courseId, int totalDurationHours)
    : courseId_(std::move(courseId)), totalDurationHours_(totalDurationHours) {}

void CourseSyllabus::addObjective(const std::string& objective) {
    learningObjectives_.push_back(objective);
}

void CourseSyllabus::addPrerequisite(const std::string& prerequisite) {
    prerequisites_.push_back(prerequisite);
}

bool CourseSyllabus::meetsPrerequisites(const std::vector<std::string>& studentSkills) const {
    for (const auto& req : prerequisites_) {
        if (std::find(studentSkills.begin(), studentSkills.end(), req) == studentSkills.end()) {
            return false;
        }
    }
    return true;
}

int CourseSyllabus::getObjectivesCount() const {
    return static_cast<int>(learningObjectives_.size());
}

} // namespace online_learning
