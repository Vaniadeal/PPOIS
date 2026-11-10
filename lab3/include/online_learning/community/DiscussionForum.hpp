#pragma once
#include <string>

namespace online_learning {

class DiscussionForum {
private:
    std::string forumId_;
    std::string courseId_;
    std::string title_;
    int threadsCount_;
    bool isModerated_;

public:
    DiscussionForum();
    DiscussionForum(std::string forumId, std::string courseId, std::string title, bool moderated);

    void recordNewThread();
    void toggleModeration(bool enabled);
    bool canPostWithoutApproval() const;
    std::string getForumStatus() const;

    const std::string& getForumId() const { return forumId_; }
    const std::string& getCourseId() const { return courseId_; }
    const std::string& getTitle() const { return title_; }
    int getThreadsCount() const { return threadsCount_; }
    bool isModerated() const { return isModerated_; }
};

} // namespace online_learning
