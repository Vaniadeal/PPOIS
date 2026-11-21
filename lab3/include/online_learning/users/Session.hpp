#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class Session {
private:
    std::string sessionId_;
    std::string userId_;
    int64_t createdAt_;
    int64_t expiresAt_;
    std::string ipAddress_;
    bool isValid_;

public:
    Session();
    Session(std::string sessionId, std::string userId, int64_t createdAt,
            int64_t durationSeconds, std::string ipAddress);

    bool isExpired(int64_t currentTimestamp) const;
    void invalidate();
    void extendSession(int64_t additionalSeconds);
    bool matchesIp(const std::string& incomingIp) const;

    const std::string& getSessionId() const { return sessionId_; }
    const std::string& getUserId() const { return userId_; }
    int64_t getCreatedAt() const { return createdAt_; }
    int64_t getExpiresAt() const { return expiresAt_; }
    const std::string& getIpAddress() const { return ipAddress_; }
    bool isValid() const { return isValid_; }
};

} // namespace online_learning
