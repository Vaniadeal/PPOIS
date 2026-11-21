#pragma once
#include <string>

namespace online_learning {

class Tag {
private:
    std::string tagId_;
    std::string name_;
    int usageCount_;

public:
    Tag();
    Tag(std::string tagId, std::string name);

    void incrementUsage();
    void decrementUsage();
    bool matchesQuery(const std::string& query) const;

    const std::string& getTagId() const { return tagId_; }
    const std::string& getName() const { return name_; }
    int getUsageCount() const { return usageCount_; }
};

} // namespace online_learning
