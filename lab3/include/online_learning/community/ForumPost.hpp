#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class ForumPost {
private:
    std::string postId_;
    std::string threadId_;
    std::string authorId_;
    std::string content_;
    int upvotesCount_;
    int64_t createdAt_;
    bool isAcceptedSolution_;

public:
    ForumPost();
    ForumPost(std::string postId, std::string threadId, std::string authorId,
              std::string content, int64_t createdAt);

    void upvote();
    void markAsAcceptedSolution();
    void editContent(const std::string& newContent);
    std::string formatPostPreview() const;

    const std::string& getPostId() const { return postId_; }
    const std::string& getThreadId() const { return threadId_; }
    const std::string& getAuthorId() const { return authorId_; }
    const std::string& getContent() const { return content_; }
    int getUpvotesCount() const { return upvotesCount_; }
    int64_t getCreatedAt() const { return createdAt_; }
    bool isAcceptedSolution() const { return isAcceptedSolution_; }
};

} // namespace online_learning
