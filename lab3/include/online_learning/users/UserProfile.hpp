#pragma once
#include <string>

namespace online_learning {

class UserProfile {
private:
    std::string avatarUrl_;
    std::string bio_;
    std::string phoneNumber_;
    std::string country_;
    std::string websiteUrl_;
    std::string timezone_;

public:
    UserProfile();
    UserProfile(std::string avatarUrl, std::string bio, std::string phoneNumber,
                std::string country, std::string websiteUrl, std::string timezone);

    void updateBio(const std::string& newBio);
    void updateContactInfo(const std::string& phone, const std::string& country);
    bool isValidTimezone() const;
    std::string formatSummary() const;

    const std::string& getAvatarUrl() const { return avatarUrl_; }
    const std::string& getBio() const { return bio_; }
    const std::string& getPhoneNumber() const { return phoneNumber_; }
    const std::string& getCountry() const { return country_; }
    const std::string& getWebsiteUrl() const { return websiteUrl_; }
    const std::string& getTimezone() const { return timezone_; }
};

} // namespace online_learning
