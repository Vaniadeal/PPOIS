#include "online_learning/users/AuthenticationService.hpp"

namespace online_learning {

AuthenticationService::AuthenticationService()
    : maxLoginAttemptsBeforeLock_(UserCredentials::kMaxFailedAttempts),
      sessionDurationSeconds_(86400), registeredUsersCount_(0) {}

AuthenticationService::AuthenticationService(int maxAttempts, int sessionDuration)
    : maxLoginAttemptsBeforeLock_(maxAttempts),
      sessionDurationSeconds_(sessionDuration), registeredUsersCount_(0) {}

bool AuthenticationService::authenticate(UserCredentials& credentials,
                                         const std::string& passwordHash,
                                         int64_t currentTimestamp) {
    if (credentials.isAccountLocked()) {
        throw AuthenticationFailedException("Account is locked due to too many failed attempts");
    }
    if (!credentials.verifyPasswordHash(passwordHash)) {
        credentials.recordFailedAttempt();
        throw AuthenticationFailedException("Invalid password credentials");
    }
    credentials.resetFailedAttempts();
    credentials.setLastLoginTimestamp(currentTimestamp);
    return true;
}

Session AuthenticationService::createSession(const std::string& sessionId,
                                            const std::string& userId,
                                            const std::string& ipAddress,
                                            int64_t currentTimestamp) {
    return Session(sessionId, userId, currentTimestamp, sessionDurationSeconds_, ipAddress);
}

void AuthenticationService::revokeSession(Session& session) {
    session.invalidate();
}

bool AuthenticationService::isSessionValid(const Session& session, int64_t currentTimestamp) const {
    return !session.isExpired(currentTimestamp);
}

void AuthenticationService::registerUserAccount() {
    registeredUsersCount_++;
}

} // namespace online_learning
