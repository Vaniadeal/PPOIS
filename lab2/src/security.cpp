#include "security.hpp"
#include <algorithm>

namespace pharma {

Permission::Permission(std::string code, std::string desc, std::string resource)
    : code_(std::move(code)), description_(std::move(desc)),
      resource_(std::move(resource)) {}

bool Permission::matches(const std::string& required) const {
    return code_ == required;
}

Role::Role(std::string id, std::string name)
    : id_(std::move(id)), name_(std::move(name)) {}

void Role::addPermission(const std::string& code) {
    permissionCodes_.push_back(code);
}
bool Role::hasPermission(const std::string& code) const {
    return std::find(permissionCodes_.begin(), permissionCodes_.end(), code)
           != permissionCodes_.end();
}

User::User(std::string id, std::string login, std::string hash, std::string roleId)
    : id_(std::move(id)), login_(std::move(login)),
      passwordHash_(std::move(hash)), roleId_(std::move(roleId)) {}

bool User::authenticate(const std::string& password) const {
    return password == passwordHash_;
}
bool User::changePassword(const std::string& oldP, const std::string& newP) {
    if (!authenticate(oldP)) return false;
    passwordHash_ = newP; return true;
}
void User::linkToEmployee(const std::string& empId) { employeeId_ = empId; }
void User::linkToCustomer(const std::string& custId) { customerId_ = custId; }

AuditLog::AuditLog(std::string id, std::string userId, Date ts,
                   std::string action, std::string details)
    : id_(std::move(id)), userId_(std::move(userId)), timestamp_(ts),
      action_(std::move(action)), details_(std::move(details)) {}

bool AuditLog::isCritical() const {
    return action_ == "delete" || action_ == "role_change";
}
std::string AuditLog::format() const { return action_ + ": " + details_; }
bool AuditLog::filterByUser(const std::string& userId) const {
    return userId_ == userId;
}

Notification::Notification(std::string id, std::string message, Date at)
    : id_(std::move(id)), message_(std::move(message)), createdAt_(at) {}

void Notification::sendToUser(const std::string& userId) { userId_ = userId; }
void Notification::sendToCustomer(const std::string& custId) {
    customerId_ = custId;
}
bool Notification::isImportant() const {
    return message_.find("urgent") != std::string::npos;
}

} 