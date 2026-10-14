#include "online_learning/courses/Course.hpp"

namespace online_learning {

Course::Course(std::string id, std::string title, std::string description,
               std::string instructorId, std::string categoryId, double price,
               std::string difficultyLevel)
    : id_(std::move(id)), title_(std::move(title)), description_(std::move(description)),
      instructorId_(std::move(instructorId)), categoryId_(std::move(categoryId)),
      price_(price), difficultyLevel_(std::move(difficultyLevel)), isPublished_(false) {}

void Course::publish() {
    isPublished_ = true;
}

void Course::unpublish() {
    isPublished_ = false;
}

void Course::updatePrice(double newPrice) {
    if (newPrice >= 0.0) {
        price_ = newPrice;
    }
}

void Course::addModuleId(const std::string& moduleId) {
    moduleIds_.push_back(moduleId);
}

void Course::addTag(const std::string& tagName) {
    tagNames_.push_back(tagName);
}

bool Course::isFreeCourse() const {
    return price_ == 0.0;
}

bool Course::matchesDifficulty(const std::string& targetLevel) const {
    return difficultyLevel_ == targetLevel;
}

} // namespace online_learning
