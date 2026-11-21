#include "online_learning/platform/OnlineLearningPlatform.hpp"

namespace online_learning {

OnlineLearningPlatform::OnlineLearningPlatform(std::string name)
    : platformName_(std::move(name)) {}

void OnlineLearningPlatform::registerStudent(const Student& student) {
    if (students_.find(student.getId()) != students_.end()) {
        throw UserAlreadyExistsException(student.getEmail());
    }
    students_.emplace(student.getId(), student);
    authService_.registerUserAccount();
}

void OnlineLearningPlatform::registerInstructor(const Instructor& instructor) {
    if (instructors_.find(instructor.getId()) != instructors_.end()) {
        throw UserAlreadyExistsException(instructor.getEmail());
    }
    instructors_.emplace(instructor.getId(), instructor);
    authService_.registerUserAccount();
}

Student& OnlineLearningPlatform::getStudent(const std::string& studentId) {
    auto it = students_.find(studentId);
    if (it == students_.end()) {
        throw UserNotFoundException(studentId);
    }
    return it->second;
}

Instructor& OnlineLearningPlatform::getInstructor(const std::string& instructorId) {
    auto it = instructors_.find(instructorId);
    if (it == instructors_.end()) {
        throw UserNotFoundException(instructorId);
    }
    return it->second;
}

bool OnlineLearningPlatform::hasStudent(const std::string& studentId) const {
    return students_.find(studentId) != students_.end();
}

void OnlineLearningPlatform::createCourse(const Course& course) {
    if (courses_.find(course.getId()) != courses_.end()) {
        throw CourseAlreadyPublishedException(course.getId());
    }
    courses_.emplace(course.getId(), course);
    auto itInst = instructors_.find(course.getInstructorId());
    if (itInst != instructors_.end()) {
        itInst->second.publishNewCourse();
    }
}

void OnlineLearningPlatform::publishCourse(const std::string& courseId) {
    auto it = courses_.find(courseId);
    if (it == courses_.end()) {
        throw CourseNotFoundException(courseId);
    }
    it->second.publish();
}

Course& OnlineLearningPlatform::getCourse(const std::string& courseId) {
    auto it = courses_.find(courseId);
    if (it == courses_.end()) {
        throw CourseNotFoundException(courseId);
    }
    return it->second;
}

std::vector<Course> OnlineLearningPlatform::getPublishedCourses() const {
    std::vector<Course> result;
    for (const auto& [id, c] : courses_) {
        if (c.isPublished()) {
            result.push_back(c);
        }
    }
    return result;
}

void OnlineLearningPlatform::addPromoCode(const PromoCode& promo) {
    promoCodes_.push_back(promo);
}

Order OnlineLearningPlatform::purchaseCourse(const std::string& studentId,
                                            const std::string& courseId,
                                            const std::string& promoCodeStr,
                                            int64_t currentTimestamp) {
    Student& student = getStudent(studentId);
    Course& course = getCourse(courseId);

    std::string orderId = "ORD-" + std::to_string(orders_.size() + 1);
    Order order(orderId, studentId, courseId, course.getPrice());

    if (!promoCodeStr.empty()) {
        bool promoFound = false;
        for (auto& promo : promoCodes_) {
            if (promo.getCode() == promoCodeStr) {
                order.applyPromoCode(promo, currentTimestamp);
                promo.applyDiscount();
                promoFound = true;
                break;
            }
        }
        if (!promoFound) {
            throw InvalidPromoCodeException(promoCodeStr, "Promo code not found");
        }
    }

    if (order.getFinalPrice() > 0.0) {
        student.withdrawFunds(order.getFinalPrice());
    }
    order.markPaid();
    orders_.emplace(order.getOrderId(), order);

    enrollStudent(studentId, courseId, currentTimestamp);

    Notification notif("NOTIF-" + std::to_string(notificationService_.getTotalSentCount() + 1),
                      studentId, "Course Purchase",
                      "Successfully enrolled in course: " + course.getTitle(), currentTimestamp);
    notificationService_.sendNotification(notif);

    return order;
}

void OnlineLearningPlatform::enrollStudent(const std::string& studentId,
                                          const std::string& courseId,
                                          int64_t timestamp) {
    Student& student = getStudent(studentId);
    getCourse(courseId);

    std::string key = studentId + ":" + courseId;
    if (enrollments_.find(key) != enrollments_.end()) {
        throw AlreadyEnrolledException(studentId, courseId);
    }

    student.addEnrolledCourse(courseId);
    std::string enrollmentId = "ENR-" + std::to_string(enrollments_.size() + 1);
    enrollments_.emplace(key, Enrollment(enrollmentId, studentId, courseId, timestamp));
}

void OnlineLearningPlatform::recordStudentProgress(const std::string& studentId,
                                                  const std::string& courseId,
                                                  double progressPercent) {
    std::string key = studentId + ":" + courseId;
    auto it = enrollments_.find(key);
    if (it == enrollments_.end()) {
        throw EnrollmentNotFoundException(studentId, courseId);
    }
    it->second.updateProgress(progressPercent);
    if (it->second.isCompleted()) {
        Student& student = getStudent(studentId);
        student.recordLessonCompleted();
    }
}

Certificate OnlineLearningPlatform::issueCertificate(const std::string& studentId,
                                                    const std::string& courseId,
                                                    int64_t timestamp,
                                                    const std::string& certId) {
    std::string key = studentId + ":" + courseId;
    auto it = enrollments_.find(key);
    if (it == enrollments_.end() || !it->second.isCompleted()) {
        throw EnrollmentNotFoundException(studentId, courseId);
    }

    if (certificates_.find(certId) != certificates_.end()) {
        throw CertificateAlreadyIssuedException(studentId, courseId);
    }

    Course& course = getCourse(courseId);
    Instructor& instructor = getInstructor(course.getInstructorId());

    std::string verifyCode = "VERIFY-" + certId;
    Certificate cert(certId, studentId, courseId, timestamp, verifyCode, instructor.getFullName());
    certificates_.emplace(certId, cert);

    Student& student = getStudent(studentId);
    student.awardCertificateId(certId);

    Notification notif("NOTIF-" + std::to_string(notificationService_.getTotalSentCount() + 1),
                      studentId, "Certificate Issued",
                      "Congratulations! You completed " + course.getTitle(), timestamp);
    notificationService_.sendNotification(notif);

    return cert;
}

bool OnlineLearningPlatform::verifyCertificate(const std::string& verificationCode) const {
    for (const auto& [id, cert] : certificates_) {
        if (cert.verify(verificationCode)) {
            return true;
        }
    }
    return false;
}

int OnlineLearningPlatform::getTotalStudentsCount() const {
    return static_cast<int>(students_.size());
}

int OnlineLearningPlatform::getTotalCoursesCount() const {
    return static_cast<int>(courses_.size());
}

int OnlineLearningPlatform::getTotalCertificatesIssuedCount() const {
    return static_cast<int>(certificates_.size());
}

} // namespace online_learning
