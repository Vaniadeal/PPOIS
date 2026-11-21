#include "online_learning/users/Administrator.hpp"

namespace online_learning {

Administrator::Administrator(std::string id, std::string email, std::string fullName,
                             int64_t regDate, int adminLevel, std::string department)
    : User(std::move(id), std::move(email), std::move(fullName), regDate),
      adminLevel_(adminLevel), department_(std::move(department)),
      auditLogCount_(0), isSuperAdmin_(adminLevel >= 3) {}

void Administrator::performAuditAction(const std::string& actionDescription) {
    (void)actionDescription;
    auditLogCount_++;
}

void Administrator::elevatePrivilege() {
    adminLevel_++;
    isSuperAdmin_ = (adminLevel_ >= 3);
}

void Administrator::revokePrivilege() {
    if (adminLevel_ > 1) {
        adminLevel_--;
    }
    isSuperAdmin_ = (adminLevel_ >= 3);
}

bool Administrator::canManageAdmins() const {
    return isSuperAdmin_ && isActive_;
}

} // namespace online_learning
