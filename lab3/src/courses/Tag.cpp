#include "online_learning/courses/Tag.hpp"

namespace online_learning {

Tag::Tag() : tagId_(""), name_(""), usageCount_(0) {}

Tag::Tag(std::string tagId, std::string name)
    : tagId_(std::move(tagId)), name_(std::move(name)), usageCount_(1) {}

void Tag::incrementUsage() {
    usageCount_++;
}

void Tag::decrementUsage() {
    if (usageCount_ > 0) {
        usageCount_--;
    }
}

bool Tag::matchesQuery(const std::string& query) const {
    return name_.find(query) != std::string::npos;
}

} // namespace online_learning
