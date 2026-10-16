#include "online_learning/community/CourseReview.hpp"
#include <algorithm>

namespace online_learning {

CourseReview::CourseReview()
    : reviewId_(""), courseId_(""), studentId_(""),
      ratingStars_(5), commentText_(""), createdAt_(0), isVerifiedPurchase_(false) {}

CourseReview::CourseReview(std::string reviewId, std::string courseId, std::string studentId,
                           int stars, std::string comment, int64_t createdAt, bool verified)
    : reviewId_(std::move(reviewId)), courseId_(std::move(courseId)),
      studentId_(std::move(studentId)), ratingStars_(std::clamp(stars, 1, 5)),
      commentText_(std::move(comment)), createdAt_(createdAt), isVerifiedPurchase_(verified) {}

void CourseReview::updateReview(int newStars, const std::string& newComment) {
    ratingStars_ = std::clamp(newStars, 1, 5);
    commentText_ = newComment;
}

bool CourseReview::isPositive() const {
    return ratingStars_ >= 4;
}

bool CourseReview::isCritical() const {
    return ratingStars_ <= 2;
}

std::string CourseReview::formatReviewLine() const {
    std::string verified = isVerifiedPurchase_ ? "[Verified]" : "[Unverified]";
    return verified + " " + std::to_string(ratingStars_) + "★: " + commentText_;
}

} // namespace online_learning
