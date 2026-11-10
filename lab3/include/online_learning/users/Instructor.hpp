#pragma once
#include <string>
#include "online_learning/users/User.hpp"
#include "online_learning/exceptions/InsufficientFundsException.hpp"

namespace online_learning {

class Instructor : public User {
private:
    std::string specialization_;
    double ratingScore_;
    int publishedCourseCount_;
    double totalEarnings_;
    bool isVerified_;

public:
    Instructor(std::string id, std::string email, std::string fullName,
               int64_t regDate, std::string specialization);

    void publishNewCourse();
    void updateRating(double newReviewScore);
    void addEarnings(double amount);
    void verifyInstructor();
    double requestPayout(double requestedAmount);

    const std::string& getSpecialization() const { return specialization_; }
    double getRatingScore() const { return ratingScore_; }
    int getPublishedCourseCount() const { return publishedCourseCount_; }
    double getTotalEarnings() const { return totalEarnings_; }
    bool isVerified() const { return isVerified_; }
};

} // namespace online_learning
