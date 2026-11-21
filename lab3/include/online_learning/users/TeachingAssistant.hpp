#pragma once
#include <vector>
#include <string>
#include "online_learning/users/User.hpp"

namespace online_learning {

class TeachingAssistant : public User {
private:
    std::vector<std::string> assignedCourseIds_;
    int reviewedSubmissionsCount_;
    bool canGradeQuizzes_;
    bool canAnswerForum_;

public:
    TeachingAssistant(std::string id, std::string email, std::string fullName, int64_t regDate);

    void assignToCourse(const std::string& courseId);
    void unassignFromCourse(const std::string& courseId);
    void recordSubmissionReviewed();
    bool hasPermissionForCourse(const std::string& courseId) const;

    const std::vector<std::string>& getAssignedCourseIds() const { return assignedCourseIds_; }
    int getReviewedSubmissionsCount() const { return reviewedSubmissionsCount_; }
    bool canGradeQuizzes() const { return canGradeQuizzes_; }
    bool canAnswerForum() const { return canAnswerForum_; }
};

} // namespace online_learning
