#include "online_learning/community/ForumPost.hpp"

namespace online_learning {

ForumPost::ForumPost()
    : postId_(""), threadId_(""), authorId_(""),
      content_(""), upvotesCount_(0), createdAt_(0), isAcceptedSolution_(false) {}

ForumPost::ForumPost(std::string postId, std::string threadId, std::string authorId,
                     std::string content, int64_t createdAt)
    : postId_(std::move(postId)), threadId_(std::move(threadId)),
      authorId_(std::move(authorId)), content_(std::move(content)),
      upvotesCount_(0), createdAt_(createdAt), isAcceptedSolution_(false) {}

void ForumPost::upvote() {
    upvotesCount_++;
}

void ForumPost::markAsAcceptedSolution() {
    isAcceptedSolution_ = true;
}

void ForumPost::editContent(const std::string& newContent) {
    content_ = newContent;
}

std::string ForumPost::formatPostPreview() const {
    std::string prefix = isAcceptedSolution_ ? "[SOLUTION] " : "";
    std::string truncated = content_.size() > 50 ? content_.substr(0, 50) + "..." : content_;
    return prefix + "(" + std::to_string(upvotesCount_) + " votes): " + truncated;
}

} // namespace online_learning
