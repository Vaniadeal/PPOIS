#pragma once
#include <string>
#include <vector>

namespace online_learning {

class Course {
private:
    std::string id_;
    std::string title_;
    std::string description_;
    std::string instructorId_;
    std::string categoryId_;
    double price_;
    std::string difficultyLevel_;
    bool isPublished_;
    std::vector<std::string> moduleIds_;
    std::vector<std::string> tagNames_;

public:
    Course(std::string id, std::string title, std::string description,
           std::string instructorId, std::string categoryId, double price,
           std::string difficultyLevel);

    void publish();
    void unpublish();
    void updatePrice(double newPrice);
    void addModuleId(const std::string& moduleId);
    void addTag(const std::string& tagName);
    bool isFreeCourse() const;
    bool matchesDifficulty(const std::string& targetLevel) const;

    const std::string& getId() const { return id_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getDescription() const { return description_; }
    const std::string& getInstructorId() const { return instructorId_; }
    const std::string& getCategoryId() const { return categoryId_; }
    double getPrice() const { return price_; }
    const std::string& getDifficultyLevel() const { return difficultyLevel_; }
    bool isPublished() const { return isPublished_; }
    const std::vector<std::string>& getModuleIds() const { return moduleIds_; }
    const std::vector<std::string>& getTagNames() const { return tagNames_; }
};

} // namespace online_learning
