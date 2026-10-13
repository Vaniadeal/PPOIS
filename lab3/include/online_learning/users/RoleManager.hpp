#pragma once
#include <string>
#include "online_learning/users/User.hpp"
#include "online_learning/exceptions/AccessDeniedException.hpp"

namespace online_learning {

class RoleManager {
private:
    std::string systemScope_;
    int totalRolesDefined_;

public:
    explicit RoleManager(std::string systemScope);

    bool canUserManageCourse(const User& user, const std::string& courseInstructorId) const;
    bool canUserGradeAssignment(const User& user) const;
    bool canUserIssueCertificate(const User& user) const;
    void validateOperationAccess(bool hasAccess, const std::string& operationName) const;

    const std::string& getSystemScope() const { return systemScope_; }
    int getTotalRolesDefined() const { return totalRolesDefined_; }
};

} // namespace online_learning
