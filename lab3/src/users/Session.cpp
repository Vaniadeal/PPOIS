#include "online_learning/users/Session.hpp"

namespace online_learning {

Session::Session()
    : sessionId_(""), userId_(""), createdAt_(0), expiresAt_(0), ipAddress_(""), isValid_(false) {}

Session::Session(std::string sessionId, std::string userId, int64_t createdAt,
                 int64_t durationSeconds, std::string ipAddress)
    : sessionId_(std::move(sessionId)), userId_(std::move(userId)),
      createdAt_(createdAt), expiresAt_(createdAt + durationSeconds),
      ipAddress_(std::move(ipAddress)), isValid_(true) {}

bool Session::isExpired(int64_t currentTimestamp) const {
    return !isValid_ || currentTimestamp >= expiresAt_;
}

void Session::invalidate() {
    isValid_ = false;
}

void Session::extendSession(int64_t additionalSeconds) {
    if (isValid_) {
        expiresAt_ += additionalSeconds;
    }
}

bool Session::matchesIp(const std::string& incomingIp) const {
    return ipAddress_ == incomingIp;
}

} // namespace online_learning
