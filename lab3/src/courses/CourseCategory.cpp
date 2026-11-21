#include "online_learning/courses/CourseCategory.hpp"

namespace online_learning {

CourseCategory::CourseCategory()
    : categoryId_(""), name_(""), slug_(""), description_(""), parentCategoryId_("") {}

CourseCategory::CourseCategory(std::string categoryId, std::string name, std::string slug,
                               std::string description, std::string parentId)
    : categoryId_(std::move(categoryId)), name_(std::move(name)),
      slug_(std::move(slug)), description_(std::move(description)),
      parentCategoryId_(std::move(parentId)) {}

bool CourseCategory::isRootCategory() const {
    return parentCategoryId_.empty();
}

void CourseCategory::updateDescription(const std::string& newDesc) {
    description_ = newDesc;
}

bool CourseCategory::matchesSlug(const std::string& slug) const {
    return slug_ == slug;
}

std::string CourseCategory::getFullCategoryPath() const {
    if (isRootCategory()) {
        return "/" + slug_;
    }
    return "/" + parentCategoryId_ + "/" + slug_;
}

} // namespace online_learning
