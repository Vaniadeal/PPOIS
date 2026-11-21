#include "online_learning/users/Student.hpp"
#include <algorithm>

namespace online_learning {

Student::Student(std::string id, std::string email, std::string fullName, int64_t regDate)
    : User(std::move(id), std::move(email), std::move(fullName), regDate),
      walletBalance_(0.0), studentLevel_(1), completedLessonsCount_(0) {}

void Student::depositFunds(double amount) {
    if (amount > 0.0) {
        walletBalance_ += amount;
    }
}

void Student::withdrawFunds(double amount) {
    if (amount <= 0.0) {
        return;
    }
    if (walletBalance_ < amount) {
        throw InsufficientFundsException(walletBalance_, amount);
    }
    walletBalance_ -= amount;
}

void Student::addEnrolledCourse(const std::string& courseId) {
    auto it = std::find(enrolledCourseIds_.begin(), enrolledCourseIds_.end(), courseId);
    if (it != enrolledCourseIds_.end()) {
        throw AlreadyEnrolledException(id_, courseId);
    }
    enrolledCourseIds_.push_back(courseId);
}

void Student::recordLessonCompleted() {
    completedLessonsCount_++;
    constexpr int kLessonsPerLevel = 10;
    studentLevel_ = 1 + (completedLessonsCount_ / kLessonsPerLevel);
}

void Student::awardCertificateId(const std::string& certificateId) {
    earnedCertificateIds_.push_back(certificateId);
}

} // namespace online_learning
