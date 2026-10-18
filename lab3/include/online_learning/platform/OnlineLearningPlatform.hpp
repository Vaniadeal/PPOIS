#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdint>

#include "online_learning/users/Student.hpp"
#include "online_learning/users/Instructor.hpp"
#include "online_learning/users/AuthenticationService.hpp"
#include "online_learning/courses/Course.hpp"
#include "online_learning/progress/Enrollment.hpp"
#include "online_learning/finance/Order.hpp"
#include "online_learning/finance/PromoCode.hpp"
#include "online_learning/certificates/Certificate.hpp"
#include "online_learning/platform/NotificationService.hpp"

#include "online_learning/exceptions/UserAlreadyExistsException.hpp"
#include "online_learning/exceptions/UserNotFoundException.hpp"
#include "online_learning/exceptions/CourseNotFoundException.hpp"
#include "online_learning/exceptions/CourseAlreadyPublishedException.hpp"
#include "online_learning/exceptions/EnrollmentNotFoundException.hpp"
#include "online_learning/exceptions/AlreadyEnrolledException.hpp"
#include "online_learning/exceptions/InvalidPromoCodeException.hpp"
#include "online_learning/exceptions/CertificateAlreadyIssuedException.hpp"

namespace online_learning {

class OnlineLearningPlatform {
private:
    std::string platformName_;
    std::map<std::string, Student> students_;
    std::map<std::string, Instructor> instructors_;
    std::map<std::string, Course> courses_;
    std::map<std::string, Enrollment> enrollments_;
    std::map<std::string, Order> orders_;
    std::map<std::string, Certificate> certificates_;
    std::vector<PromoCode> promoCodes_;
    NotificationService notificationService_;
    AuthenticationService authService_;

public:
    explicit OnlineLearningPlatform(std::string name);

    void registerStudent(const Student& student);
    void registerInstructor(const Instructor& instructor);
    Student& getStudent(const std::string& studentId);
    Instructor& getInstructor(const std::string& instructorId);
    bool hasStudent(const std::string& studentId) const;

    void createCourse(const Course& course);
    void publishCourse(const std::string& courseId);
    Course& getCourse(const std::string& courseId);
    std::vector<Course> getPublishedCourses() const;

    void addPromoCode(const PromoCode& promo);
    Order purchaseCourse(const std::string& studentId, const std::string& courseId,
                         const std::string& promoCodeStr, int64_t currentTimestamp);
    void enrollStudent(const std::string& studentId, const std::string& courseId, int64_t timestamp);
    void recordStudentProgress(const std::string& studentId, const std::string& courseId, double progressPercent);

    Certificate issueCertificate(const std::string& studentId, const std::string& courseId,
                                int64_t timestamp, const std::string& certId);
    bool verifyCertificate(const std::string& verificationCode) const;

    int getTotalStudentsCount() const;
    int getTotalCoursesCount() const;
    int getTotalCertificatesIssuedCount() const;
    NotificationService& getNotificationService() { return notificationService_; }

    const std::string& getPlatformName() const { return platformName_; }
};

} // namespace online_learning
