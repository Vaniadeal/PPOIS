#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class CertificateAlreadyIssuedException : public PlatformException {
public:
    explicit CertificateAlreadyIssuedException(const std::string& studentId, const std::string& courseId)
        : PlatformException("Certificate already issued for student " + studentId +
                            " in course " + courseId) {}
};

} // namespace online_learning
