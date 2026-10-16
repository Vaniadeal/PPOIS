#pragma once
#include <string>

namespace online_learning {

class StudentGrade {
private:
    std::string studentId_;
    std::string courseId_;
    double totalPointsAccumulated_;
    char finalGradeLetter_;
    bool isCoursePassed_;

public:
    StudentGrade();
    StudentGrade(std::string studentId, std::string courseId);

    void recordPoints(double additionalPoints);
    void computeFinalGrade(double maxPossiblePoints, double passingThreshold);
    bool isHonorRollGrade() const;
    std::string formatGradeCard() const;

    const std::string& getStudentId() const { return studentId_; }
    const std::string& getCourseId() const { return courseId_; }
    double getTotalPointsAccumulated() const { return totalPointsAccumulated_; }
    char getFinalGradeLetter() const { return finalGradeLetter_; }
    bool isCoursePassed() const { return isCoursePassed_; }
};

} // namespace online_learning
