#include "online_learning/users/RoleManager.hpp"

namespace online_learning {

RoleManager::RoleManager(std::string systemScope)
    : systemScope_(std::move(systemScope)), totalRolesDefined_(4) {}

bool RoleManager::canUserManageCourse(const User& user, const std::string& courseInstructorId) const {
    return user.getId() == courseInstructorId && user.isActive();
}

bool RoleManager::canUserGradeAssignment(const User& user) const {
    return user.isActive();
}

bool RoleManager::canUserIssueCertificate(const User& user) const {
    return user.isActive();
}

void RoleManager::validateOperationAccess(bool hasAccess, const std::string& operationName) const {
    if (!hasAccess) {
        throw AccessDeniedException(operationName);
    }
}

} // namespace online_learning
