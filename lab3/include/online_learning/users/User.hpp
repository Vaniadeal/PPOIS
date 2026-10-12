#pragma once
#include <string>
#include <cstdint>
#include "online_learning/users/UserProfile.hpp"

namespace online_learning {

class User {
protected:
    std::string id_;
    std::string email_;
    std::string fullName_;
    int64_t registrationDate_;
    bool isActive_;
    UserProfile profile_;

public:
    User(std::string id, std::string email, std::string fullName, int64_t registrationDate);
    virtual ~User() = default;

    void deactivate();
    void activate();
    void updateProfile(const UserProfile& newProfile);
    std::string getDisplayTitle() const;

    const std::string& getId() const { return id_; }
    const std::string& getEmail() const { return email_; }
    const std::string& getFullName() const { return fullName_; }
    int64_t getRegistrationDate() const { return registrationDate_; }
    bool isActive() const { return isActive_; }
    const UserProfile& getProfile() const { return profile_; }
};

} // namespace online_learning
