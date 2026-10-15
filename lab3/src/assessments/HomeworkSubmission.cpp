#include "online_learning/assessments/HomeworkSubmission.hpp"

namespace online_learning {

HomeworkSubmission::HomeworkSubmission()
    : submissionId_(""), assignmentId_(""), studentId_(""),
      submissionText_(""), attachmentUrl_(""), submissionTimestamp_(0), isGraded_(false) {}

HomeworkSubmission::HomeworkSubmission(std::string submissionId, std::string assignmentId,
                                       std::string studentId, std::string text,
                                       std::string attachmentUrl, int64_t timestamp)
    : submissionId_(std::move(submissionId)), assignmentId_(std::move(assignmentId)),
      studentId_(std::move(studentId)), submissionText_(std::move(text)),
      attachmentUrl_(std::move(attachmentUrl)), submissionTimestamp_(timestamp), isGraded_(false) {}

void HomeworkSubmission::markAsGraded() {
    isGraded_ = true;
}

bool HomeworkSubmission::hasAttachment() const {
    return !attachmentUrl_.empty();
}

void HomeworkSubmission::updateSubmission(const std::string& newText, const std::string& newUrl) {
    submissionText_ = newText;
    attachmentUrl_ = newUrl;
}

std::string HomeworkSubmission::formatSubmissionHeader() const {
    return "Submission ID: " + submissionId_ + " by Student: " + studentId_;
}

} // namespace online_learning
