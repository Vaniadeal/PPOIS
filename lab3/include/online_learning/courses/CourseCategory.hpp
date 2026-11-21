#pragma once
#include <string>

namespace online_learning {

class CourseCategory {
private:
    std::string categoryId_;
    std::string name_;
    std::string slug_;
    std::string description_;
    std::string parentCategoryId_;

public:
    CourseCategory();
    CourseCategory(std::string categoryId, std::string name, std::string slug,
                   std::string description, std::string parentId = "");

    bool isRootCategory() const;
    void updateDescription(const std::string& newDesc);
    bool matchesSlug(const std::string& slug) const;
    std::string getFullCategoryPath() const;

    const std::string& getCategoryId() const { return categoryId_; }
    const std::string& getName() const { return name_; }
    const std::string& getSlug() const { return slug_; }
    const std::string& getDescription() const { return description_; }
    const std::string& getParentCategoryId() const { return parentCategoryId_; }
};

} // namespace online_learning
