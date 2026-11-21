#pragma once
#include <string>
#include <vector>
#include "online_learning/progress/StudentGrade.hpp"

namespace online_learning {

class GradeBook {
private:
    std::string courseId_;
    double totalMaxPoints_;
    std::string gradeScaleType_;
    double passingGradePoints_;
    std::vector<StudentGrade> studentGrades_;

public:
    GradeBook();
    GradeBook(std::string courseId, double maxPoints, double passingGrade, std::string scaleType);

    void addOrUpdateStudentGrade(const StudentGrade& grade);
    double calculateAverageScore() const;
    int getPassingStudentsCount() const;
    bool isStudentPassing(const std::string& studentId) const;
    int getTotalEnrolledGradedCount() const;

    const std::string& getCourseId() const { return courseId_; }
    double getTotalMaxPoints() const { return totalMaxPoints_; }
    const std::string& getGradeScaleType() const { return gradeScaleType_; }
    double getPassingGradePoints() const { return passingGradePoints_; }
    const std::vector<StudentGrade>& getStudentGrades() const { return studentGrades_; }
};

} // namespace online_learning
