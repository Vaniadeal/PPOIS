#include "online_learning/users/Instructor.hpp"

namespace online_learning {

Instructor::Instructor(std::string id, std::string email, std::string fullName,
                       int64_t regDate, std::string specialization)
    : User(std::move(id), std::move(email), std::move(fullName), regDate),
      specialization_(std::move(specialization)), ratingScore_(5.0),
      publishedCourseCount_(0), totalEarnings_(0.0), isVerified_(false) {}

void Instructor::publishNewCourse() {
    publishedCourseCount_++;
}

void Instructor::updateRating(double newReviewScore) {
    constexpr double kAlpha = 0.2;
    ratingScore_ = (1.0 - kAlpha) * ratingScore_ + kAlpha * newReviewScore;
}

void Instructor::addEarnings(double amount) {
    if (amount > 0.0) {
        totalEarnings_ += amount;
    }
}

void Instructor::verifyInstructor() {
    isVerified_ = true;
}

double Instructor::requestPayout(double requestedAmount) {
    if (requestedAmount > totalEarnings_ || requestedAmount <= 0.0) {
        throw InsufficientFundsException(totalEarnings_, requestedAmount);
    }
    totalEarnings_ -= requestedAmount;
    return requestedAmount;
}

} // namespace online_learning
