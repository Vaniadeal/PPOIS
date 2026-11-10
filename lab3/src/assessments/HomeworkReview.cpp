#include "online_learning/assessments/HomeworkReview.hpp"

namespace online_learning {

HomeworkReview::HomeworkReview()
    : reviewId_(""), submissionId_(""), reviewerId_(""),
      score_(0), feedbackText_(""), reviewDate_(0) {}

HomeworkReview::HomeworkReview(std::string reviewId, std::string submissionId,
                               std::string reviewerId, int score,
                               std::string feedback, int64_t reviewDate)
    : reviewId_(std::move(reviewId)), submissionId_(std::move(submissionId)),
      reviewerId_(std::move(reviewerId)), score_(score),
      feedbackText_(std::move(feedback)), reviewDate_(reviewDate) {}

void HomeworkReview::updateFeedback(const std::string& newFeedback) {
    feedbackText_ = newFeedback;
}

bool HomeworkReview::isPerfectScore(int maxScore) const {
    return score_ >= maxScore;
}

bool HomeworkReview::isPassing(int passingScore) const {
    return score_ >= passingScore;
}

std::string HomeworkReview::formatReviewCard() const {
    return "Score: " + std::to_string(score_) + " | Feedback: " + feedbackText_;
}

} // namespace online_learning
