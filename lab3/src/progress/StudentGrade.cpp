#include "online_learning/progress/StudentGrade.hpp"

namespace online_learning {

StudentGrade::StudentGrade()
    : studentId_(""), courseId_(""), totalPointsAccumulated_(0.0),
      finalGradeLetter_('F'), isCoursePassed_(false) {}

StudentGrade::StudentGrade(std::string studentId, std::string courseId)
    : studentId_(std::move(studentId)), courseId_(std::move(courseId)),
      totalPointsAccumulated_(0.0), finalGradeLetter_('F'), isCoursePassed_(false) {}

void StudentGrade::recordPoints(double additionalPoints) {
    if (additionalPoints > 0.0) {
        totalPointsAccumulated_ += additionalPoints;
    }
}

void StudentGrade::computeFinalGrade(double maxPossiblePoints, double passingThreshold) {
    if (maxPossiblePoints <= 0.0) {
        finalGradeLetter_ = 'F';
        isCoursePassed_ = false;
        return;
    }
    double percent = (totalPointsAccumulated_ / maxPossiblePoints) * 100.0;
    isCoursePassed_ = (percent >= passingThreshold);

    if (percent >= 90.0) {
        finalGradeLetter_ = 'A';
    } else if (percent >= 80.0) {
        finalGradeLetter_ = 'B';
    } else if (percent >= 70.0) {
        finalGradeLetter_ = 'C';
    } else if (percent >= 60.0) {
        finalGradeLetter_ = 'D';
    } else {
        finalGradeLetter_ = 'F';
    }
}

bool StudentGrade::isHonorRollGrade() const {
    return finalGradeLetter_ == 'A';
}

std::string StudentGrade::formatGradeCard() const {
    std::string outcome = isCoursePassed_ ? "PASSED" : "FAILED";
    return "Grade: " + std::string(1, finalGradeLetter_) + " (" + std::to_string(totalPointsAccumulated_) +
           " pts) - " + outcome;
}

} // namespace online_learning
