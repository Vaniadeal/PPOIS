#pragma once
#include "core.hpp"

namespace pharma {

class Permission {
    std::string code_;
    std::string description_;
    std::string resource_;
public:
    Permission(std::string code, std::string desc, std::string resource);
    bool matches(const std::string& required) const;
    const std::string& code() const { return code_; }
};

class Role {
    std::string id_;
    std::string name_;
    std::vector<std::string> permissionCodes_;
public:
    Role(std::string id, std::string name);
    void addPermission(const std::string& code);
    bool hasPermission(const std::string& code) const;
    size_t permissionCount() const { return permissionCodes_.size(); }
};

class User {
    std::string id_;
    std::string login_;
    std::string passwordHash_;
    std::string roleId_;
    std::string employeeId_;
    std::string customerId_;
public:
    User(std::string id, std::string login, std::string hash,
         std::string roleId);
    bool authenticate(const std::string& password) const;
    bool changePassword(const std::string& oldPwd, const std::string& newPwd);
    void linkToEmployee(const std::string& empId);
    void linkToCustomer(const std::string& custId);
};

class AuditLog {
    std::string id_;
    std::string userId_;
    Date timestamp_;
    std::string action_;
    std::string details_;
public:
    AuditLog(std::string id, std::string userId, Date ts, std::string action,
             std::string details);
    bool isCritical() const;
    std::string format() const;
    bool filterByUser(const std::string& userId) const;
};

class Notification {
    std::string id_;
    std::string userId_;
    std::string customerId_;
    std::string employeeId_;
    std::string message_;
    Date createdAt_;
public:
    Notification(std::string id, std::string message, Date at);
    void sendToUser(const std::string& userId);
    void sendToCustomer(const std::string& custId);
    bool isImportant() const;
};

} 