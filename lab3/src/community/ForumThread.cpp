#include "online_learning/community/ForumThread.hpp"

namespace online_learning {

ForumThread::ForumThread()
    : threadId_(""), forumId_(""), authorId_(""),
      title_(""), createdAt_(0), isPinned_(false), isClosed_(false) {}

ForumThread::ForumThread(std::string threadId, std::string forumId, std::string authorId,
                         std::string title, int64_t createdAt)
    : threadId_(std::move(threadId)), forumId_(std::move(forumId)),
      authorId_(std::move(authorId)), title_(std::move(title)),
      createdAt_(createdAt), isPinned_(false), isClosed_(false) {}

void ForumThread::addPost(const ForumPost& post) {
    if (!isClosed_) {
        posts_.push_back(post);
    }
}

void ForumThread::setPinned(bool pinned) {
    isPinned_ = pinned;
}

void ForumThread::closeThread() {
    isClosed_ = true;
}

bool ForumThread::hasAcceptedSolution() const {
    for (const auto& post : posts_) {
        if (post.isAcceptedSolution()) {
            return true;
        }
    }
    return false;
}

int ForumThread::getTotalRepliesCount() const {
    return static_cast<int>(posts_.size());
}

} // namespace online_learning
