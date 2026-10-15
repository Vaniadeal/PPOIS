#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class HomeworkSubmission {
private:
    std::string submissionId_;
    std::string assignmentId_;
    std::string studentId_;
    std::string submissionText_;
    std::string attachmentUrl_;
    int64_t submissionTimestamp_;
    bool isGraded_;

public:
    HomeworkSubmission();
    HomeworkSubmission(std::string submissionId, std::string assignmentId,
                       std::string studentId, std::string text,
                       std::string attachmentUrl, int64_t timestamp);

    void markAsGraded();
    bool hasAttachment() const;
    void updateSubmission(const std::string& newText, const std::string& newUrl);
    std::string formatSubmissionHeader() const;

    const std::string& getSubmissionId() const { return submissionId_; }
    const std::string& getAssignmentId() const { return assignmentId_; }
    const std::string& getStudentId() const { return studentId_; }
    const std::string& getSubmissionText() const { return submissionText_; }
    const std::string& getAttachmentUrl() const { return attachmentUrl_; }
    int64_t getSubmissionTimestamp() const { return submissionTimestamp_; }
    bool isGraded() const { return isGraded_; }
};

} // namespace online_learning
