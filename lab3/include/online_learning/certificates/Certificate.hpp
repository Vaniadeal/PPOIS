#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class Certificate {
private:
    std::string certificateId_;
    std::string studentId_;
    std::string courseId_;
    int64_t issueDate_;
    std::string verificationCode_;
    std::string instructorSignature_;
    bool isValid_;

public:
    Certificate();
    Certificate(std::string certId, std::string studentId, std::string courseId,
                int64_t issueDate, std::string verificationCode, std::string signature);

    bool verify(const std::string& code) const;
    void revoke();
    std::string getVerificationUrl() const;
    std::string formatCertificateText() const;

    const std::string& getCertificateId() const { return certificateId_; }
    const std::string& getStudentId() const { return studentId_; }
    const std::string& getCourseId() const { return courseId_; }
    int64_t getIssueDate() const { return issueDate_; }
    const std::string& getVerificationCode() const { return verificationCode_; }
    const std::string& getInstructorSignature() const { return instructorSignature_; }
    bool isValid() const { return isValid_; }
};

} // namespace online_learning
