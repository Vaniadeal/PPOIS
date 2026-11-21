#include "online_learning/users/User.hpp"

namespace online_learning {

User::User(std::string id, std::string email, std::string fullName, int64_t registrationDate)
    : id_(std::move(id)), email_(std::move(email)), fullName_(std::move(fullName)),
      registrationDate_(registrationDate), isActive_(true) {}

void User::deactivate() {
    isActive_ = false;
}

void User::activate() {
    isActive_ = true;
}

void User::updateProfile(const UserProfile& newProfile) {
    profile_ = newProfile;
}

std::string User::getDisplayTitle() const {
    return fullName_ + " (" + email_ + ")";
}

} // namespace online_learning
