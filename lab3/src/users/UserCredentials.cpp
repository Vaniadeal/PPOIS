#include "online_learning/users/UserCredentials.hpp"

namespace online_learning {

UserCredentials::UserCredentials()
    : login_(""), passwordHash_(""), salt_(""), lastLoginTimestamp_(0), failedLoginAttempts_(0) {}

UserCredentials::UserCredentials(std::string login, std::string passwordHash, std::string salt)
    : login_(std::move(login)), passwordHash_(std::move(passwordHash)),
      salt_(std::move(salt)), lastLoginTimestamp_(0), failedLoginAttempts_(0) {}

bool UserCredentials::verifyPasswordHash(const std::string& candidateHash) const {
    if (isAccountLocked()) {
        return false;
    }
    return passwordHash_ == candidateHash;
}

void UserCredentials::recordFailedAttempt() {
    failedLoginAttempts_++;
}

void UserCredentials::resetFailedAttempts() {
    failedLoginAttempts_ = 0;
}

bool UserCredentials::isAccountLocked() const {
    return failedLoginAttempts_ >= kMaxFailedAttempts;
}

} // namespace online_learning
