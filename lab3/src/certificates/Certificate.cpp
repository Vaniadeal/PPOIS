#include "online_learning/certificates/Certificate.hpp"

namespace online_learning {

Certificate::Certificate()
    : certificateId_(""), studentId_(""), courseId_(""),
      issueDate_(0), verificationCode_(""), instructorSignature_(""), isValid_(false) {}

Certificate::Certificate(std::string certId, std::string studentId, std::string courseId,
                         int64_t issueDate, std::string verificationCode, std::string signature)
    : certificateId_(std::move(certId)), studentId_(std::move(studentId)),
      courseId_(std::move(courseId)), issueDate_(issueDate),
      verificationCode_(std::move(verificationCode)),
      instructorSignature_(std::move(signature)), isValid_(true) {}

bool Certificate::verify(const std::string& code) const {
    return isValid_ && verificationCode_ == code;
}

void Certificate::revoke() {
    isValid_ = false;
}

std::string Certificate::getVerificationUrl() const {
    return "https://verify.academy.org/cert/" + verificationCode_;
}

std::string Certificate::formatCertificateText() const {
    std::string validStr = isValid_ ? "VALID" : "REVOKED";
    return "Certificate #" + certificateId_ + " awarded to " + studentId_ +
           " for Course " + courseId_ + " [" + validStr + "]";
}

} // namespace online_learning
