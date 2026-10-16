#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class CourseReview {
private:
    std::string reviewId_;
    std::string courseId_;
    std::string studentId_;
    int ratingStars_;
    std::string commentText_;
    int64_t createdAt_;
    bool isVerifiedPurchase_;

public:
    CourseReview();
    CourseReview(std::string reviewId, std::string courseId, std::string studentId,
                 int stars, std::string comment, int64_t createdAt, bool verified);

    void updateReview(int newStars, const std::string& newComment);
    bool isPositive() const;
    bool isCritical() const;
    std::string formatReviewLine() const;

    const std::string& getReviewId() const { return reviewId_; }
    const std::string& getCourseId() const { return courseId_; }
    const std::string& getStudentId() const { return studentId_; }
    int getRatingStars() const { return ratingStars_; }
    const std::string& getCommentText() const { return commentText_; }
    int64_t getCreatedAt() const { return createdAt_; }
    bool isVerifiedPurchase() const { return isVerifiedPurchase_; }
};

} // namespace online_learning
