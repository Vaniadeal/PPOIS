#pragma once
#include <string>
#include "online_learning/users/User.hpp"

namespace online_learning {

class Administrator : public User {
private:
    int adminLevel_;
    std::string department_;
    int auditLogCount_;
    bool isSuperAdmin_;

public:
    Administrator(std::string id, std::string email, std::string fullName,
                  int64_t regDate, int adminLevel, std::string department);

    void performAuditAction(const std::string& actionDescription);
    void elevatePrivilege();
    void revokePrivilege();
    bool canManageAdmins() const;

    int getAdminLevel() const { return adminLevel_; }
    const std::string& getDepartment() const { return department_; }
    int getAuditLogCount() const { return auditLogCount_; }
    bool isSuperAdmin() const { return isSuperAdmin_; }
};

} // namespace online_learning
