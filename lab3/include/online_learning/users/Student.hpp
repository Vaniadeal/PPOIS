#pragma once
#include <vector>
#include "online_learning/users/User.hpp"
#include "online_learning/exceptions/InsufficientFundsException.hpp"
#include "online_learning/exceptions/AlreadyEnrolledException.hpp"

namespace online_learning {

class Student : public User {
private:
    double walletBalance_;
    int studentLevel_;
    int completedLessonsCount_;
    std::vector<std::string> enrolledCourseIds_;
    std::vector<std::string> earnedCertificateIds_;

public:
    Student(std::string id, std::string email, std::string fullName, int64_t regDate);

    void depositFunds(double amount);
    void withdrawFunds(double amount);
    void addEnrolledCourse(const std::string& courseId);
    void recordLessonCompleted();
    void awardCertificateId(const std::string& certificateId);

    double getWalletBalance() const { return walletBalance_; }
    int getStudentLevel() const { return studentLevel_; }
    int getCompletedLessonsCount() const { return completedLessonsCount_; }
    const std::vector<std::string>& getEnrolledCourseIds() const { return enrolledCourseIds_; }
    const std::vector<std::string>& getEarnedCertificateIds() const { return earnedCertificateIds_; }
};

} // namespace online_learning
