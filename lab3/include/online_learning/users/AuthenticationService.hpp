#pragma once
#include <string>
#include <cstdint>
#include "online_learning/users/UserCredentials.hpp"
#include "online_learning/users/Session.hpp"
#include "online_learning/exceptions/AuthenticationFailedException.hpp"

namespace online_learning {

class AuthenticationService {
private:
    int maxLoginAttemptsBeforeLock_;
    int sessionDurationSeconds_;
    int registeredUsersCount_;

public:
    AuthenticationService();
    AuthenticationService(int maxAttempts, int sessionDuration);

    bool authenticate(UserCredentials& credentials, const std::string& passwordHash, int64_t currentTimestamp);
    Session createSession(const std::string& sessionId, const std::string& userId,
                          const std::string& ipAddress, int64_t currentTimestamp);
    void revokeSession(Session& session);
    bool isSessionValid(const Session& session, int64_t currentTimestamp) const;
    void registerUserAccount();

    int getMaxLoginAttempts() const { return maxLoginAttemptsBeforeLock_; }
    int getSessionDuration() const { return sessionDurationSeconds_; }
    int getRegisteredUsersCount() const { return registeredUsersCount_; }
};

} // namespace online_learning
