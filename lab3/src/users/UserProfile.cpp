#include "online_learning/users/UserProfile.hpp"

namespace online_learning {

UserProfile::UserProfile()
    : avatarUrl_("default_avatar.png"), bio_(""), phoneNumber_(""),
      country_("Unknown"), websiteUrl_(""), timezone_("UTC") {}

UserProfile::UserProfile(std::string avatarUrl, std::string bio, std::string phoneNumber,
                         std::string country, std::string websiteUrl, std::string timezone)
    : avatarUrl_(std::move(avatarUrl)), bio_(std::move(bio)),
      phoneNumber_(std::move(phoneNumber)), country_(std::move(country)),
      websiteUrl_(std::move(websiteUrl)), timezone_(std::move(timezone)) {}

void UserProfile::updateBio(const std::string& newBio) {
    bio_ = newBio;
}

void UserProfile::updateContactInfo(const std::string& phone, const std::string& country) {
    phoneNumber_ = phone;
    country_ = country;
}

bool UserProfile::isValidTimezone() const {
    return timezone_ == "UTC" || timezone_.rfind("GMT", 0) == 0 || timezone_.find('/') != std::string::npos;
}

std::string UserProfile::formatSummary() const {
    return "Country: " + country_ + ", TZ: " + timezone_ + ", Bio: " + bio_;
}

} // namespace online_learning
