#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "online_learning/community/ForumPost.hpp"

namespace online_learning {

class ForumThread {
private:
    std::string threadId_;
    std::string forumId_;
    std::string authorId_;
    std::string title_;
    int64_t createdAt_;
    bool isPinned_;
    bool isClosed_;
    std::vector<ForumPost> posts_;

public:
    ForumThread();
    ForumThread(std::string threadId, std::string forumId, std::string authorId,
                std::string title, int64_t createdAt);

    void addPost(const ForumPost& post);
    void setPinned(bool pinned);
    void closeThread();
    bool hasAcceptedSolution() const;
    int getTotalRepliesCount() const;

    const std::string& getThreadId() const { return threadId_; }
    const std::string& getForumId() const { return forumId_; }
    const std::string& getAuthorId() const { return authorId_; }
    const std::string& getTitle() const { return title_; }
    int64_t getCreatedAt() const { return createdAt_; }
    bool isPinned() const { return isPinned_; }
    bool isClosed() const { return isClosed_; }
    const std::vector<ForumPost>& getPosts() const { return posts_; }
};

} // namespace online_learning
