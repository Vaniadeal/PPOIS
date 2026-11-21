#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class UserCredentials {
private:
    std::string login_;
    std::string passwordHash_;
    std::string salt_;
    int64_t lastLoginTimestamp_;
    int failedLoginAttempts_;

public:
    static constexpr int kMaxFailedAttempts = 5;

    UserCredentials();
    UserCredentials(std::string login, std::string passwordHash, std::string salt);

    bool verifyPasswordHash(const std::string& candidateHash) const;
    void recordFailedAttempt();
    void resetFailedAttempts();
    bool isAccountLocked() const;

    const std::string& getLogin() const { return login_; }
    const std::string& getPasswordHash() const { return passwordHash_; }
    const std::string& getSalt() const { return salt_; }
    int64_t getLastLoginTimestamp() const { return lastLoginTimestamp_; }
    int getFailedLoginAttempts() const { return failedLoginAttempts_; }
    void setLastLoginTimestamp(int64_t timestamp) { lastLoginTimestamp_ = timestamp; }
};

} // namespace online_learning
