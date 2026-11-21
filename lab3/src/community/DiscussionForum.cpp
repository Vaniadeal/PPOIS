#include "online_learning/community/DiscussionForum.hpp"

namespace online_learning {

DiscussionForum::DiscussionForum()
    : forumId_(""), courseId_(""), title_(""), threadsCount_(0), isModerated_(false) {}

DiscussionForum::DiscussionForum(std::string forumId, std::string courseId, std::string title, bool moderated)
    : forumId_(std::move(forumId)), courseId_(std::move(courseId)),
      title_(std::move(title)), threadsCount_(0), isModerated_(moderated) {}

void DiscussionForum::recordNewThread() {
    threadsCount_++;
}

void DiscussionForum::toggleModeration(bool enabled) {
    isModerated_ = enabled;
}

bool DiscussionForum::canPostWithoutApproval() const {
    return !isModerated_;
}

std::string DiscussionForum::getForumStatus() const {
    std::string mod = isModerated_ ? "Moderated" : "Open";
    return title_ + " [" + mod + "] - " + std::to_string(threadsCount_) + " threads";
}

} // namespace online_learning
