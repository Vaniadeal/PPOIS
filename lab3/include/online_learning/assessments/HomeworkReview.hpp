#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class HomeworkReview {
private:
    std::string reviewId_;
    std::string submissionId_;
    std::string reviewerId_;
    int score_;
    std::string feedbackText_;
    int64_t reviewDate_;

public:
    HomeworkReview();
    HomeworkReview(std::string reviewId, std::string submissionId,
                   std::string reviewerId, int score,
                   std::string feedback, int64_t reviewDate);

    void updateFeedback(const std::string& newFeedback);
    bool isPerfectScore(int maxScore) const;
    bool isPassing(int passingScore) const;
    std::string formatReviewCard() const;

    const std::string& getReviewId() const { return reviewId_; }
    const std::string& getSubmissionId() const { return submissionId_; }
    const std::string& getReviewerId() const { return reviewerId_; }
    int getScore() const { return score_; }
    const std::string& getFeedbackText() const { return feedbackText_; }
    int64_t getReviewDate() const { return reviewDate_; }
};

} // namespace online_learning
