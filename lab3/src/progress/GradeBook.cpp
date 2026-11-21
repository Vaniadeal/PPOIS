#include "online_learning/progress/GradeBook.hpp"

namespace online_learning {

GradeBook::GradeBook()
    : courseId_(""), totalMaxPoints_(100.0), gradeScaleType_("Standard"), passingGradePoints_(60.0) {}

GradeBook::GradeBook(std::string courseId, double maxPoints, double passingGrade, std::string scaleType)
    : courseId_(std::move(courseId)), totalMaxPoints_(maxPoints),
      gradeScaleType_(std::move(scaleType)), passingGradePoints_(passingGrade) {}

void GradeBook::addOrUpdateStudentGrade(const StudentGrade& grade) {
    for (auto& existing : studentGrades_) {
        if (existing.getStudentId() == grade.getStudentId()) {
            existing = grade;
            return;
        }
    }
    studentGrades_.push_back(grade);
}

double GradeBook::calculateAverageScore() const {
    if (studentGrades_.empty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (const auto& g : studentGrades_) {
        sum += g.getTotalPointsAccumulated();
    }
    return sum / static_cast<double>(studentGrades_.size());
}

int GradeBook::getPassingStudentsCount() const {
    int count = 0;
    for (const auto& g : studentGrades_) {
        if (g.isCoursePassed()) {
            count++;
        }
    }
    return count;
}

bool GradeBook::isStudentPassing(const std::string& studentId) const {
    for (const auto& g : studentGrades_) {
        if (g.getStudentId() == studentId) {
            return g.isCoursePassed();
        }
    }
    return false;
}

int GradeBook::getTotalEnrolledGradedCount() const {
    return static_cast<int>(studentGrades_.size());
}

} // namespace online_learning
