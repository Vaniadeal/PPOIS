#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class Enrollment {
public:
    enum class Status {
        Active,
        Completed,
        Dropped,
        Suspended
    };

private:
    std::string enrollmentId_;
    std::string studentId_;
    std::string courseId_;
    int64_t enrolledDate_;
    int64_t completionDate_;
    double progressPercent_;
    Status status_;

public:
    Enrollment();
    Enrollment(std::string enrollmentId, std::string studentId,
               std::string courseId, int64_t enrolledDate);

    void updateProgress(double newPercent);
    void completeCourse(int64_t completionDate);
    void dropCourse();
    bool isCompleted() const;
    bool isActive() const;
    std::string getStatusString() const;

    const std::string& getEnrollmentId() const { return enrollmentId_; }
    const std::string& getStudentId() const { return studentId_; }
    const std::string& getCourseId() const { return courseId_; }
    int64_t getEnrolledDate() const { return enrolledDate_; }
    int64_t getCompletionDate() const { return completionDate_; }
    double getProgressPercent() const { return progressPercent_; }
    Status getStatus() const { return status_; }
};

} // namespace online_learning
