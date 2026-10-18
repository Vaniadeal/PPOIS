#include <UnitTest++/UnitTest++.h>
#include "online_learning/platform.hpp"

using namespace online_learning;

SUITE(UsersAndSecurityTests) {

TEST(UserProfile_BasicFunctionality) {
    UserProfile profile("avatar.png", "Passionate coder", "+123456789", "Belarus", "https://kruk.dev", "UTC");
    CHECK_EQUAL("avatar.png", profile.getAvatarUrl());
    CHECK_EQUAL("Passionate coder", profile.getBio());
    CHECK(profile.isValidTimezone());

    profile.updateBio("Updated bio");
    CHECK_EQUAL("Updated bio", profile.getBio());

    profile.updateContactInfo("+987654321", "Poland");
    CHECK_EQUAL("+987654321", profile.getPhoneNumber());
    CHECK_EQUAL("Poland", profile.getCountry());
    CHECK(!profile.formatSummary().empty());
}

TEST(UserCredentials_AuthVerificationAndLockout) {
    UserCredentials creds("ivan_user", "hash123", "salt456");
    CHECK_EQUAL("ivan_user", creds.getLogin());
    CHECK(!creds.isAccountLocked());
    CHECK(creds.verifyPasswordHash("hash123"));
    CHECK(!creds.verifyPasswordHash("wrong_hash"));

    for (int i = 0; i < UserCredentials::kMaxFailedAttempts; ++i) {
        creds.recordFailedAttempt();
    }
    CHECK(creds.isAccountLocked());
    CHECK(!creds.verifyPasswordHash("hash123")); // locked accounts cannot authenticate

    creds.resetFailedAttempts();
    CHECK(!creds.isAccountLocked());
    CHECK_EQUAL(0, creds.getFailedLoginAttempts());
}

TEST(User_ActivationAndDeactivation) {
    User user("u-1", "user@test.org", "Test User", 1700000000);
    CHECK(user.isActive());
    CHECK_EQUAL("u-1", user.getId());
    CHECK_EQUAL("user@test.org", user.getEmail());
    CHECK_EQUAL("Test User", user.getFullName());

    user.deactivate();
    CHECK(!user.isActive());
    user.activate();
    CHECK(user.isActive());
    CHECK(!user.getDisplayTitle().empty());
}

TEST(Student_WalletAndLeveling) {
    Student student("s-1", "student@bsuir.by", "Ivan Student", 1700000000);
    CHECK_CLOSE(0.0, student.getWalletBalance(), 0.001);
    CHECK_EQUAL(1, student.getStudentLevel());

    student.depositFunds(100.0);
    CHECK_CLOSE(100.0, student.getWalletBalance(), 0.001);

    student.withdrawFunds(40.0);
    CHECK_CLOSE(60.0, student.getWalletBalance(), 0.001);

    CHECK_THROW(student.withdrawFunds(999.0), InsufficientFundsException);

    student.addEnrolledCourse("c-1");
    CHECK_EQUAL(1, student.getEnrolledCourseIds().size());
    CHECK_THROW(student.addEnrolledCourse("c-1"), AlreadyEnrolledException);

    for (int i = 0; i < 20; ++i) {
        student.recordLessonCompleted();
    }
    CHECK(student.getStudentLevel() >= 3);
    student.awardCertificateId("cert-1");
    CHECK_EQUAL(1, student.getEarnedCertificateIds().size());
}

TEST(Instructor_EarningsAndRatings) {
    Instructor inst("inst-1", "prof@uni.edu", "Prof Smith", 1690000000, "Math");
    CHECK_CLOSE(5.0, inst.getRatingScore(), 0.001);
    CHECK(!inst.isVerified());

    inst.verifyInstructor();
    CHECK(inst.isVerified());

    inst.publishNewCourse();
    CHECK_EQUAL(1, inst.getPublishedCourseCount());

    inst.updateRating(4.0);
    CHECK(inst.getRatingScore() < 5.0);

    inst.addEarnings(200.0);
    CHECK_CLOSE(200.0, inst.getTotalEarnings(), 0.001);

    double payout = inst.requestPayout(50.0);
    CHECK_CLOSE(50.0, payout, 0.001);
    CHECK_CLOSE(150.0, inst.getTotalEarnings(), 0.001);
    CHECK_THROW(inst.requestPayout(500.0), InsufficientFundsException);
}

TEST(TeachingAssistant_Permissions) {
    TeachingAssistant ta("ta-1", "ta@uni.edu", "Alex TA", 1700000000);
    ta.assignToCourse("c-1");
    CHECK(ta.hasPermissionForCourse("c-1"));
    CHECK(!ta.hasPermissionForCourse("c-2"));

    ta.recordSubmissionReviewed();
    CHECK_EQUAL(1, ta.getReviewedSubmissionsCount());

    ta.unassignFromCourse("c-1");
    CHECK(!ta.hasPermissionForCourse("c-1"));
}

TEST(Administrator_Privileges) {
    Administrator admin("admin-1", "root@academy.org", "Root Admin", 1680000000, 3, "IT");
    CHECK(admin.canManageAdmins());
    CHECK(admin.isSuperAdmin());

    admin.performAuditAction("Checked system logs");
    CHECK_EQUAL(1, admin.getAuditLogCount());

    admin.revokePrivilege();
    admin.revokePrivilege();
    CHECK(!admin.isSuperAdmin());
    admin.elevatePrivilege();
    admin.elevatePrivilege();
    CHECK(admin.isSuperAdmin());
}

TEST(RoleManager_AccessChecks) {
    RoleManager rm("GlobalSystem");
    User normalUser("u-1", "u@test.org", "Normal", 1700000000);
    CHECK(rm.canUserManageCourse(normalUser, "u-1"));
    CHECK(!rm.canUserManageCourse(normalUser, "other-instructor"));
    CHECK(rm.canUserGradeAssignment(normalUser));
    CHECK(rm.canUserIssueCertificate(normalUser));

    CHECK_THROW(rm.validateOperationAccess(false, "DELETE_DATABASE"), AccessDeniedException);
}

TEST(Session_Lifecycle) {
    Session session("sess-1", "u-1", 1000, 3600, "127.0.0.1");
    CHECK(!session.isExpired(2000));
    CHECK(session.isExpired(5000));
    CHECK(session.matchesIp("127.0.0.1"));
    CHECK(!session.matchesIp("192.168.1.1"));

    session.extendSession(3600);
    CHECK(!session.isExpired(5000));
    session.invalidate();
    CHECK(!session.isValid());
}

TEST(AuthenticationService_Workflow) {
    AuthenticationService auth(3, 3600);
    UserCredentials creds("john", "secret_hash", "salt");

    CHECK(auth.authenticate(creds, "secret_hash", 1000));
    CHECK_THROW(auth.authenticate(creds, "bad_hash", 1000), AuthenticationFailedException);

    Session s = auth.createSession("s-100", "u-1", "10.0.0.1", 1000);
    CHECK(auth.isSessionValid(s, 2000));
    auth.revokeSession(s);
    CHECK(!auth.isSessionValid(s, 2000));
}

} // SUITE

SUITE(CoursesAndContentTests) {

TEST(Tag_Usage) {
    Tag tag("tag-1", "cpp17");
    CHECK_EQUAL("cpp17", tag.getName());
    CHECK(tag.matchesQuery("cpp"));
    tag.incrementUsage();
    CHECK_EQUAL(2, tag.getUsageCount());
    tag.decrementUsage();
    CHECK_EQUAL(1, tag.getUsageCount());
}

TEST(CourseCategory_Hierarchy) {
    CourseCategory cat("cat-1", "Computer Science", "cs", "All CS courses");
    CHECK(cat.isRootCategory());
    CHECK(cat.matchesSlug("cs"));
    CHECK_EQUAL("/cs", cat.getFullCategoryPath());

    CourseCategory subCat("sub-1", "Algorithms", "algo", "Algo courses", "cs");
    CHECK(!subCat.isRootCategory());
    CHECK_EQUAL("/cs/algo", subCat.getFullCategoryPath());
}

TEST(Attachment_SizeAndType) {
    Attachment att("att-1", "cheatsheet.pdf", 2097152, "application/pdf", "https://files.org/1.pdf");
    CHECK(att.isPdfDocument());
    CHECK_CLOSE(2.0, att.getFileSizeMegaBytes(), 0.01);
    CHECK(att.isExceedingSizeLimit(1000000));
    CHECK(!att.isExceedingSizeLimit(5000000));
}

TEST(Lesson_HierarchyAndTypes) {
    Lesson generic("l-1", "m-1", "Intro", 10, 1, true);
    CHECK_EQUAL("GenericLesson", generic.getLessonType());
    generic.addAttachment(Attachment("a-1", "notes.txt", 100, "text/plain", "url"));
    CHECK_EQUAL(1, generic.getAttachments().size());
    generic.reorder(2);
    CHECK_EQUAL(2, generic.getOrderIndex());

    VideoLesson video("l-2", "m-1", "Video Lecture", 20, 2, false,
                      "https://cdn.org/video.mp4", "1080p", true, 1200);
    CHECK_EQUAL("VideoLesson", video.getLessonType());
    CHECK(video.isHighDefinition());
    CHECK(video.hasSubtitles());
    CHECK_EQUAL("20m 0s", video.formatDurationString());

    TextArticleLesson article("l-3", "m-1", "Guide", 3, true,
                              "This is a sample markdown text article lesson for students.");
    CHECK_EQUAL("TextArticleLesson", article.getLessonType());
    CHECK(article.isShortRead());
    CHECK(article.getWordCount() > 5);

    InteractiveCodingLesson coding("l-4", "m-1", "Hello World Task", 15, 4, false,
                                   "int main() {}", "cpp", "assert(true);", 30);
    CHECK_EQUAL("InteractiveCodingLesson", coding.getLessonType());
    CHECK(coding.isLanguageSupported("cpp"));
    CHECK(coding.validateTimeLimit());
}

TEST(CourseModule_Lessons) {
    CourseModule mod("mod-1", "course-1", "Foundations", 1, true, 75.0);
    CHECK_EQUAL(0, mod.getTotalLessonsCount());
    mod.addLessonId("l-1");
    mod.addLessonId("l-2");
    CHECK_EQUAL(2, mod.getTotalLessonsCount());
    CHECK(mod.isEligibleForCompletion(80.0));
    CHECK(!mod.isEligibleForCompletion(60.0));
    mod.removeLessonId("l-1");
    CHECK_EQUAL(1, mod.getTotalLessonsCount());
}

TEST(CourseSyllabus_Prerequisites) {
    CourseSyllabus syllabus("c-1", 40);
    syllabus.addObjective("Learn C++17");
    syllabus.addPrerequisite("Basic C knowledge");
    CHECK_EQUAL(1, syllabus.getObjectivesCount());
    CHECK(syllabus.meetsPrerequisites({"Basic C knowledge", "Git"}));
    CHECK(!syllabus.meetsPrerequisites({"Java"}));
}

TEST(Course_Operations) {
    Course course("c-1", "C++ Deep Dive", "Complete guide", "inst-1", "cat-cs", 50.0, "Intermediate");
    CHECK(!course.isPublished());
    CHECK(!course.isFreeCourse());
    CHECK(course.matchesDifficulty("Intermediate"));

    course.publish();
    CHECK(course.isPublished());
    course.addModuleId("mod-1");
    course.addTag("C++");
    CHECK_EQUAL(1, course.getModuleIds().size());
    CHECK_EQUAL(1, course.getTagNames().size());

    course.updatePrice(0.0);
    CHECK(course.isFreeCourse());
    course.unpublish();
    CHECK(!course.isPublished());
}

} // SUITE

SUITE(AssessmentsAndHomeworkTests) {

TEST(SingleChoiceQuestion_Evaluation) {
    SingleChoiceQuestion q("q-1", "quiz-1", "Pick 2", 10, {"A", "B", "C"}, 1);
    CHECK_EQUAL(3, q.getOptionsCount());
    CHECK(q.evaluateAnswer("1"));
    CHECK(!q.evaluateAnswer("0"));
    CHECK(!q.evaluateAnswer("not-a-number"));
    q.updatePoints(20);
    CHECK_EQUAL(20, q.getPoints());
    CHECK(!q.getSummary().empty());
}

TEST(MultipleChoiceQuestion_Evaluation) {
    // Mask: binary 101 = 5 (options 0 and 2 are correct)
    MultipleChoiceQuestion q("q-2", "quiz-1", "Pick 1 and 3", 10, {"A", "B", "C"}, 5);
    CHECK(q.isOptionCorrect(0));
    CHECK(!q.isOptionCorrect(1));
    CHECK(q.isOptionCorrect(2));
    CHECK_EQUAL(2, q.getCorrectCount());
    CHECK(q.evaluateAnswer("5"));
    CHECK(!q.evaluateAnswer("1"));
}

TEST(CodingQuestion_Evaluation) {
    CodingQuestion q("q-3", "quiz-1", "Implement sum", 25, "int sum(int a, int b);", "Hello 42\n", 500);
    CHECK(q.evaluateAnswer("Hello 42\n"));
    CHECK(!q.evaluateAnswer("Hello 41\n"));
    q.updateStarterCode("// write code here");
    CHECK_EQUAL("// write code here", q.getStarterCode());
}

TEST(QuizAttempt_AnswersAndScoring) {
    QuizAttempt attempt("att-1", "s-1", "quiz-1", 1000);
    StudentAnswer a1("ans-1", "att-1", "q-1", "1");
    a1.grade(true, 15);
    attempt.recordAnswer(a1);
    CHECK(a1.hasEarnedPoints());
    CHECK(!a1.formatAnswerResult().empty());

    attempt.finalizeAttempt(90, true, 1600);
    CHECK_EQUAL(90, attempt.getScorePercentage());
    CHECK(attempt.isPassed());
    CHECK_EQUAL(15, attempt.calculateTotalEarnedPoints());
    CHECK_EQUAL(600, attempt.getDurationSeconds());
}

TEST(Quiz_AttemptsConstraint) {
    Quiz quiz("quiz-1", "lesson-1", "Final Quiz", 30, 2, 70);
    quiz.addQuestionId("q-1");
    quiz.addQuestionId("q-2");
    CHECK_EQUAL(2, quiz.getTotalQuestionsCount());
    CHECK(quiz.canStudentAttempt(0));
    CHECK(quiz.canStudentAttempt(1));
    CHECK(!quiz.canStudentAttempt(2));
    CHECK(quiz.isScorePassing(75));
    CHECK(!quiz.isScorePassing(65));
}

TEST(Homework_Lifecycle) {
    HomeworkAssignment hw("hw-1", "lesson-1", "Build CLI", 100, 2000, false);
    CHECK(!hw.isDeadlinePassed(1500));
    CHECK(hw.isDeadlinePassed(2500));
    hw.extendDeadline(1000);
    CHECK(!hw.isDeadlinePassed(2500));
    CHECK(hw.getTimeRemainingSeconds(2500) > 0);

    HomeworkSubmission sub("sub-1", "hw-1", "s-1", "My solution code", "https://github.com/repo", 2200);
    CHECK(!sub.isGraded());
    CHECK(sub.hasAttachment());
    sub.markAsGraded();
    CHECK(sub.isGraded());

    HomeworkReview review("rev-1", "sub-1", "inst-1", 95, "Great design!", 2300);
    CHECK(review.isPassing(70));
    CHECK(!review.isPerfectScore(100));
    review.updateFeedback("Perfect score!");
    CHECK(!review.formatReviewCard().empty());
}

} // SUITE

SUITE(ProgressAndAnalyticsTests) {

TEST(LessonProgress_TimeAndCompletion) {
    LessonProgress lp("lp-1", "enr-1", "lesson-1", 1000);
    CHECK(!lp.isCompleted());
    lp.addTimeSpent(120);
    CHECK(lp.hasMinimumTimeSpent(100));
    lp.markCompleted(1200);
    CHECK(lp.isCompleted());
    CHECK(!lp.formatProgressStatus().empty());
}

TEST(Enrollment_ProgressAndDrop) {
    Enrollment enr("enr-1", "s-1", "c-1", 1000);
    CHECK(enr.isActive());
    enr.updateProgress(50.0);
    CHECK_CLOSE(50.0, enr.getProgressPercent(), 0.001);
    enr.updateProgress(100.0);
    CHECK(enr.isCompleted());

    Enrollment enr2("enr-2", "s-2", "c-1", 1000);
    enr2.dropCourse();
    CHECK(!enr2.isActive());
    CHECK_EQUAL("Dropped", enr2.getStatusString());
}

TEST(GradeBook_Scoring) {
    GradeBook gb("c-1", 100.0, 60.0, "Percentage");
    StudentGrade g1("s-1", "c-1");
    g1.recordPoints(95.0);
    g1.computeFinalGrade(100.0, 60.0);
    CHECK(g1.isCoursePassed());
    CHECK(g1.isHonorRollGrade());
    CHECK_EQUAL('A', g1.getFinalGradeLetter());

    StudentGrade g2("s-2", "c-1");
    g2.recordPoints(40.0);
    g2.computeFinalGrade(100.0, 60.0);
    CHECK(!g2.isCoursePassed());
    CHECK_EQUAL('F', g2.getFinalGradeLetter());

    gb.addOrUpdateStudentGrade(g1);
    gb.addOrUpdateStudentGrade(g2);
    CHECK_EQUAL(2, gb.getTotalEnrolledGradedCount());
    CHECK_EQUAL(1, gb.getPassingStudentsCount());
    CHECK(gb.isStudentPassing("s-1"));
    CHECK(!gb.isStudentPassing("s-2"));
    CHECK_CLOSE(67.5, gb.calculateAverageScore(), 0.001);
}

TEST(LearningStreakTracker_Streaks) {
    LearningStreakTracker tracker("s-1");
    tracker.recordDailyActivity(100000);
    tracker.recordDailyActivity(100000 + 86400);
    CHECK_EQUAL(2, tracker.getCurrentStreakDays());
    CHECK(tracker.isStreakActive(100000 + 86400));
    tracker.resetStreak();
    CHECK_EQUAL(0, tracker.getCurrentStreakDays());
}

TEST(CourseAnalytics_Aggregation) {
    CourseAnalytics analytics("c-1");
    analytics.incrementEnrollment();
    analytics.incrementEnrollment();
    analytics.addRevenue(200.0);
    analytics.recalculateCompletionRate(1);
    analytics.updateRatingAverage(4.5);

    CHECK_EQUAL(2, analytics.getEnrolledStudentsCount());
    CHECK_CLOSE(200.0, analytics.getTotalRevenue(), 0.001);
    CHECK_CLOSE(50.0, analytics.getCompletionRatePercent(), 0.001);
    CHECK(!analytics.generateSummaryReport().empty());
}

TEST(EngagementMetric_DropOff) {
    EngagementMetric metric("m-1", "c-1");
    metric.recordPageView();
    metric.recordPageView();
    metric.updateAverageTimeSpent(15.5);
    metric.setDropOffPoint("lesson-5");
    CHECK_EQUAL("lesson-5", metric.getDropOffLessonId());
    CHECK(metric.hasHighDropOffRate(100));
}

} // SUITE

SUITE(CommunityAndSupportTests) {

TEST(CourseReview_Stars) {
    CourseReview rev("rev-1", "c-1", "s-1", 5, "Brilliant course!", 1000, true);
    CHECK(rev.isPositive());
    CHECK(!rev.isCritical());
    rev.updateReview(1, "Outdated content");
    CHECK(rev.isCritical());
    CHECK(!rev.formatReviewLine().empty());
}

TEST(Forum_ThreadsAndPosts) {
    DiscussionForum forum("f-1", "c-1", "General", false);
    CHECK(forum.canPostWithoutApproval());
    forum.toggleModeration(true);
    CHECK(!forum.canPostWithoutApproval());
    forum.recordNewThread();
    CHECK_EQUAL(1, forum.getThreadsCount());

    ForumThread thread("t-1", "f-1", "u-1", "How to install CMake?", 1000);
    ForumPost post1("p-1", "t-1", "u-2", "Use apt install cmake", 1100);
    post1.upvote();
    post1.markAsAcceptedSolution();
    CHECK(post1.isAcceptedSolution());
    CHECK_EQUAL(1, post1.getUpvotesCount());

    thread.addPost(post1);
    CHECK_EQUAL(1, thread.getTotalRepliesCount());
    CHECK(thread.hasAcceptedSolution());
    thread.setPinned(true);
    CHECK(thread.isPinned());
    thread.closeThread();
    CHECK(thread.isClosed());
}

TEST(SupportTicket_Handling) {
    SupportTicket ticket("t-1", "u-1", "Payment issue", "Card declined", SupportTicket::Priority::High);
    CHECK(ticket.isUrgent());
    ticket.assignToAdmin("admin-99");
    CHECK_EQUAL("admin-99", ticket.getAssignedAdminId());
    ticket.addResponse(TicketResponse("r-1", "t-1", "admin-99", "Fixed on our side", 1200));
    CHECK_EQUAL(1, ticket.getResponses().size());
    ticket.resolveTicket();
    CHECK_EQUAL("Resolved", ticket.getStatusString());
}

} // SUITE

SUITE(FinanceAndMonetizationTests) {

TEST(PaymentTransaction_Statuses) {
    PaymentTransaction tx("tx-1", "u-1", 99.0, "USD", "Stripe", 1000);
    CHECK(!tx.isCompleted());
    tx.markSuccessful();
    CHECK(tx.isCompleted());
    CHECK_EQUAL("Success", tx.getStatusString());
    tx.markRefunded();
    CHECK_EQUAL("Refunded", tx.getStatusString());
}

TEST(PromoCode_Discounts) {
    PromoCode promo("PROMO50", 50.0, 2000, 2);
    CHECK(promo.isValid(1500));
    CHECK(!promo.isValid(2500)); // expired
    CHECK_CLOSE(50.0, promo.calculateDiscountAmount(100.0), 0.001);

    promo.applyDiscount();
    CHECK(promo.isValid(1500));
    promo.applyDiscount();
    CHECK(!promo.isValid(1500)); // exhausted uses
}

TEST(Order_PromoApplication) {
    Order order("ord-1", "s-1", "c-1", 100.0);
    PromoCode promo("SALE20", 20.0, 5000, 10);
    order.applyPromoCode(promo, 1000);
    CHECK_CLOSE(80.0, order.getFinalPrice(), 0.001);
    CHECK_CLOSE(20.0, order.getDiscountAmount(), 0.001);

    order.markPaid();
    CHECK(order.isPaid());
    CHECK(!order.formatReceipt().empty());

    PromoCode expiredPromo("OLD", 10.0, 500, 10);
    Order order2("ord-2", "s-1", "c-1", 100.0);
    CHECK_THROW(order2.applyPromoCode(expiredPromo, 1000), InvalidPromoCodeException);
}

TEST(SubscriptionPlan_AndUserSub) {
    SubscriptionPlan plan("sub-plan-1", "Pro Tier", 29.99, 10, 2);
    CHECK(plan.permitsCourseCount(5));
    CHECK(!plan.permitsCourseCount(15));

    UserSubscription sub("usub-1", "u-1", "sub-plan-1", 1000, 4000);
    CHECK(sub.isActive(2000));
    CHECK(!sub.isActive(5000));
    sub.renew(3000);
    CHECK(sub.isActive(5000));
    sub.cancelAutoRenewal();
    CHECK(!sub.hasAutoRenew());
}

TEST(RefundRequest_Eligibility) {
    RefundRequest refund("ref-1", "ord-1", "Not satisfied", 1000, 80.0);
    CHECK(refund.isEligibleForAutoApproval(900, 1000));
    CHECK(!refund.isEligibleForAutoApproval(900, 900 + 20 * 86400));
    refund.approve();
    CHECK(refund.isApproved());
}

TEST(PayoutRecord_Commissions) {
    PayoutRecord payout("pay-1", "inst-1", 1000.0, 0.2, 5000);
    CHECK_CLOSE(800.0, payout.calculateNetAmount(), 0.001);
    CHECK_CLOSE(200.0, payout.calculateCommissionFee(), 0.001);
    CHECK(!payout.isProcessed());
    payout.processPayout();
    CHECK(payout.isProcessed());
}

} // SUITE

SUITE(CertificatesAndPlatformFacadeTests) {

TEST(Certificate_VerificationAndRevocation) {
    CertificateTemplate tmpl("t-1", "BSUIR Academy", "Landscape", "sig.png", "Gold");
    CHECK(tmpl.isLandscapeLayout());

    Certificate cert("c-1", "s-1", "c-100", 1000, "VERIFY-XYZ", "Prof Smith");
    CHECK(cert.verify("VERIFY-XYZ"));
    CHECK(!cert.verify("WRONG"));
    cert.revoke();
    CHECK(!cert.verify("VERIFY-XYZ"));
}

TEST(AchievementBadge_Awards) {
    AchievementBadge badge("b-1", "Fast Learner", "Complete 5 lessons in a day", "icon.png", 5);
    CHECK(badge.isEligible(6));
    CHECK(!badge.isEligible(4));

    StudentBadgeAward award("a-1", "s-1", "b-1", 1500);
    CHECK(award.matchesStudent("s-1"));
    CHECK(!award.matchesStudent("s-2"));
}

TEST(Notifications_Dispatch) {
    NotificationService service;
    Notification n1("notif-1", "user-1", "Welcome", "Welcome to platform", 1000);
    service.sendNotification(n1);
    CHECK_EQUAL(1, service.getUnreadCountForRecipient("user-1"));
    CHECK_EQUAL(0, service.getUnreadCountForRecipient("user-2"));

    service.markNotificationAsRead("notif-1");
    CHECK_EQUAL(0, service.getUnreadCountForRecipient("user-1"));
}

TEST(OnlineLearningPlatform_EndToEndWorkflow) {
    OnlineLearningPlatform platform("Master Academy");

    Student student("s-1", "student@bsuir.by", "Ivan Kruk", 1000);
    student.depositFunds(200.0);
    platform.registerStudent(student);
    CHECK(platform.hasStudent("s-1"));
    CHECK_THROW(platform.registerStudent(student), UserAlreadyExistsException);

    Instructor inst("inst-1", "teacher@bsuir.by", "Teacher Pro", 1000, "Informatics");
    platform.registerInstructor(inst);
    CHECK_THROW(platform.registerInstructor(inst), UserAlreadyExistsException);

    Course course("course-1", "Advanced C++", "Master modern C++", "inst-1", "cat-cs", 100.0, "Advanced");
    platform.createCourse(course);
    platform.publishCourse("course-1");
    CHECK_EQUAL(1, platform.getPublishedCourses().size());

    PromoCode promo("DISC20", 20.0, 5000, 5);
    platform.addPromoCode(promo);

    Order order = platform.purchaseCourse("s-1", "course-1", "DISC20", 1500);
    CHECK(order.isPaid());
    CHECK_CLOSE(80.0, order.getFinalPrice(), 0.001);
    CHECK_CLOSE(120.0, platform.getStudent("s-1").getWalletBalance(), 0.001);

    // Check duplicate enrollment throws
    CHECK_THROW(platform.enrollStudent("s-1", "course-1", 2000), AlreadyEnrolledException);

    // Progress
    platform.recordStudentProgress("s-1", "course-1", 100.0);

    // Certificate issuance
    Certificate cert = platform.issueCertificate("s-1", "course-1", 2500, "CERT-001");
    CHECK(platform.verifyCertificate(cert.getVerificationCode()));
    CHECK_EQUAL(1, platform.getTotalCertificatesIssuedCount());

    // Duplicate certificate throws
    CHECK_THROW(platform.issueCertificate("s-1", "course-1", 2600, "CERT-001"), CertificateAlreadyIssuedException);

    // Notification check
    CHECK(platform.getNotificationService().getTotalSentCount() >= 2);
}

} // SUITE

SUITE(DeepCoverageSuite) {

TEST(Certificates_DeepCoverage) {
    CertificateTemplate tDefault;
    CHECK_EQUAL("Online Learning Academy", tDefault.getOrganizationName());
    tDefault.updateOrganization("Custom Academy");
    CHECK_EQUAL("Custom Academy", tDefault.getOrganizationName());
    CHECK(!tDefault.formatHeader().empty());

    Certificate cDefault;
    CHECK(!cDefault.isValid());
    Certificate certReal("c-1", "s-1", "course-1", 1000, "VCODE", "Signature");
    CHECK(!certReal.formatCertificateText().empty());
    CHECK(!certReal.getVerificationUrl().empty());

    AchievementBadge bDefault;
    CHECK_EQUAL(0, bDefault.getRequiredMilestoneValue());
    bDefault.updateDescription("New desc");
    CHECK(!bDefault.formatBadgeBanner().empty());

    StudentBadgeAward aDefault;
    CHECK_EQUAL(0, aDefault.getAwardedDate());
    StudentBadgeAward aReal("a-1", "s-1", "b-1", 1000);
    CHECK(!aReal.formatAwardLog().empty());
}

TEST(Community_DeepCoverage) {
    CourseReview rDefault;
    CHECK(rDefault.getCommentText().empty());

    ForumPost pDefault;
    CHECK_EQUAL(0, pDefault.getUpvotesCount());
    ForumPost pReal("p-1", "t-1", "u-1", "Short text", 1000);
    pReal.editContent("New content longer than fifty characters to test truncation in preview properly");
    CHECK(!pReal.formatPostPreview().empty());

    DiscussionForum fDefault;
    CHECK_EQUAL(0, fDefault.getThreadsCount());

    TicketResponse respDefault;
    CHECK(respDefault.getMessage().empty());
    TicketResponse respReal("r-1", "t-1", "u-1", "Hi", 1000);
    respReal.updateMessage("Updated msg");
    CHECK_EQUAL("Updated msg", respReal.getMessage());
    CHECK(respReal.isFromAdmin("u-1"));
    CHECK(!respReal.isFromAdmin("u-2"));
    CHECK(!respReal.formatMessageBubble().empty());

    SupportTicket tDefault;
    CHECK_EQUAL("Open", tDefault.getStatusString());
    SupportTicket tReal("t-1", "u-1", "Subj", "Desc", SupportTicket::Priority::Low);
    CHECK(!tReal.isUrgent());
    tReal.assignToAdmin("a-1");
    CHECK_EQUAL("InProgress", tReal.getStatusString());
    tReal.resolveTicket();
    CHECK_EQUAL("Resolved", tReal.getStatusString());
}

TEST(Finance_DeepCoverage) {
    PaymentTransaction txDefault;
    CHECK_EQUAL("Pending", txDefault.getStatusString());
    txDefault.markFailed();
    CHECK_EQUAL("Failed", txDefault.getStatusString());

    PromoCode pDefault;
    CHECK(!pDefault.isActive());
    PromoCode pReal("P20", 20.0, 5000, 10);
    pReal.deactivate();
    CHECK(!pReal.isActive());

    Order oDefault;
    CHECK(!oDefault.isPaid());
    Order oReal("o-1", "s-1", "c-1", 50.0);
    oReal.cancelOrder();
    CHECK(!oReal.isPaid());

    SubscriptionPlan planDefault;
    CHECK(!planDefault.isAvailable());
    SubscriptionPlan planReal("plan-1", "VIP", 99.0, 50, 3);
    planReal.updatePrice(89.0);
    CHECK_CLOSE(89.0, planReal.getMonthlyPrice(), 0.01);
    planReal.toggleAvailability(false);
    CHECK(!planReal.isAvailable());
    CHECK(!planReal.getTierDescription().empty());

    UserSubscription subDefault;
    CHECK(!subDefault.hasAutoRenew());
    UserSubscription subReal("sub-1", "u-1", "p-1", 1000, 3000);
    CHECK(subReal.getRemainingSeconds(2000) > 0);
    CHECK_EQUAL(0, subReal.getRemainingSeconds(4000));
    CHECK(!subReal.formatStatus().empty());

    RefundRequest refDefault;
    CHECK(!refDefault.isApproved());
    RefundRequest refReal("ref-1", "o-1", "Reason", 1000, 50.0);
    refReal.reject();
    CHECK(!refReal.isApproved());
    CHECK(!refReal.formatSummary().empty());

    PayoutRecord payDefault;
    CHECK(!payDefault.isProcessed());
    PayoutRecord payReal("p-1", "inst-1", 500.0, 0.1, 1000);
    CHECK(!payReal.formatPayoutReceipt().empty());
}

TEST(Progress_DeepCoverage) {
    LessonProgress lpDefault;
    CHECK_EQUAL(0, lpDefault.getTimeSpentSeconds());

    Enrollment enrDefault;
    CHECK_EQUAL("Active", enrDefault.getStatusString());
    Enrollment enrReal("e-1", "s-1", "c-1", 1000);
    enrReal.completeCourse(2000);
    CHECK(enrReal.isCompleted());
    CHECK_EQUAL("Completed", enrReal.getStatusString());

    StudentGrade sgDefault;
    CHECK_EQUAL('F', sgDefault.getFinalGradeLetter());

    StudentGrade sgB("s-1", "c-1");
    sgB.recordPoints(85.0);
    sgB.computeFinalGrade(100.0, 60.0);
    CHECK_EQUAL('B', sgB.getFinalGradeLetter());

    StudentGrade sgC("s-2", "c-1");
    sgC.recordPoints(75.0);
    sgC.computeFinalGrade(100.0, 60.0);
    CHECK_EQUAL('C', sgC.getFinalGradeLetter());

    StudentGrade sgD("s-3", "c-1");
    sgD.recordPoints(65.0);
    sgD.computeFinalGrade(100.0, 60.0);
    CHECK_EQUAL('D', sgD.getFinalGradeLetter());

    StudentGrade sgZero("s-4", "c-1");
    sgZero.computeFinalGrade(0.0, 60.0);
    CHECK_EQUAL('F', sgZero.getFinalGradeLetter());
    CHECK(!sgB.formatGradeCard().empty());

    GradeBook gbDefault;
    CHECK_EQUAL(0, gbDefault.getTotalEnrolledGradedCount());
    CHECK_CLOSE(0.0, gbDefault.calculateAverageScore(), 0.001);

    LearningStreakTracker lstDefault;
    CHECK_EQUAL(0, lstDefault.getCurrentStreakDays());

    CourseAnalytics caDefault;
    CHECK_EQUAL(0, caDefault.getEnrolledStudentsCount());

    EngagementMetric emDefault;
    CHECK_EQUAL(0, emDefault.getViewsCount());
    EngagementMetric emReal("em-1", "c-1");
    emReal.recordPageView();
    emReal.recordPageView();
    emReal.updateAverageTimeSpent(10.0);
}

TEST(Assessments_DeepCoverage) {
    SingleChoiceQuestion scq("q-1", "quiz-1", "Choice?", 10, {"A"}, 0);
    scq.addOption("B");
    CHECK_EQUAL(2, scq.getOptionsCount());

    MultipleChoiceQuestion mcq("q-2", "quiz-1", "Multi?", 10, {"A", "B"}, 3);
    CHECK(!mcq.isOptionCorrect(-1));
    CHECK(!mcq.isOptionCorrect(35));

    StudentAnswer saDefault;
    CHECK_EQUAL(0, saDefault.getEarnedPoints());
    saDefault.grade(false, 10);
    CHECK(!saDefault.formatAnswerResult().empty());

    QuizAttempt qaDefault;
    CHECK_EQUAL(0, qaDefault.getScorePercentage());

    Quiz qDefault;
    CHECK_EQUAL(0, qDefault.getTimeLimitMinutes());

    HomeworkAssignment haDefault;
    CHECK_EQUAL(0, haDefault.getDeadlineTimestamp());
    haDefault.updatePrompt("Updated prompt");
    CHECK_EQUAL("Updated prompt", haDefault.getTaskPrompt());

    HomeworkSubmission hsDefault;
    CHECK(!hsDefault.isGraded());
    hsDefault.updateSubmission("New solution", "https://newurl.com");
    CHECK(!hsDefault.formatSubmissionHeader().empty());

    HomeworkReview hrDefault;
    CHECK_EQUAL(0, hrDefault.getScore());
}

TEST(Courses_DeepCoverage) {
    Tag tDefault;
    CHECK_EQUAL(0, tDefault.getUsageCount());

    CourseCategory ccDefault;
    CHECK(ccDefault.isRootCategory());
    ccDefault.updateDescription("New category desc");
    CHECK_EQUAL("New category desc", ccDefault.getDescription());

    Attachment attDefault;
    CHECK_EQUAL(0, attDefault.getFileSizeBytes());

    Lesson lesson("l-1", "m-1", "L1", 10, 1, false);
    lesson.setFreePreview(true);
    CHECK(lesson.isFreePreview());

    VideoLesson vl("vl-1", "m-1", "V1", 10, 1, true, "url", "720p", true, 600);
    vl.updateResolution("4K");
    CHECK(vl.isHighDefinition());

    TextArticleLesson tal("tal-1", "m-1", "T1", 1, true, "Word one two three");
    tal.updateContent("New longer markdown content with many words for test");
    CHECK(tal.getWordCount() > 5);

    InteractiveCodingLesson icl("icl-1", "m-1", "I1", 10, 1, true, "code", "python", "test", 60);
    icl.updateTestScript("assert 1 == 1");
    CHECK_EQUAL("assert 1 == 1", icl.getTestScript());

    CourseModule cmDefault;
    CHECK_EQUAL(0, cmDefault.getTotalLessonsCount());
    cmDefault.updatePassingScore(80.0);
    CHECK_CLOSE(80.0, cmDefault.getPassingScorePercentage(), 0.01);

    CourseSyllabus csDefault;
    CHECK_EQUAL(0, csDefault.getTotalDurationHours());
}

TEST(Users_DeepCoverage) {
    UserProfile upDefault;
    CHECK(!upDefault.getAvatarUrl().empty());

    UserCredentials ucDefault;
    CHECK_EQUAL(0, ucDefault.getLastLoginTimestamp());

    TeachingAssistant ta("ta-1", "ta@edu.by", "TA Name", 1000);
    CHECK(ta.canGradeQuizzes());
    CHECK(ta.canAnswerForum());

    Administrator admin("adm-1", "adm@edu.by", "Admin Name", 1000, 3, "Security");
    CHECK(admin.canManageAdmins());
    admin.deactivate();
    CHECK(!admin.canManageAdmins());

    Session sDefault;
    CHECK(!sDefault.isValid());

    AuthenticationService asDefault;
    CHECK_EQUAL(UserCredentials::kMaxFailedAttempts, asDefault.getMaxLoginAttempts());
}

TEST(Platform_DeepCoverage) {
    Notification nDefault;
    CHECK(!nDefault.formatNotificationBrief().empty());

    NotificationService ns;
    ns.sendNotification(Notification("n-1", "u-1", "T1", "M1", 1000));
    ns.sendNotification(Notification("n-2", "u-1", "T2", "M2", 2000));
    CHECK_EQUAL(2, ns.getNotificationsForRecipient("u-1").size());

    OnlineLearningPlatform platform("TestPlatform");
    CHECK_EQUAL("TestPlatform", platform.getPlatformName());
    CHECK_EQUAL(0, platform.getTotalCoursesCount());

    CHECK_THROW(platform.getStudent("non-existent"), UserNotFoundException);
    CHECK_THROW(platform.getInstructor("non-existent"), UserNotFoundException);
    CHECK_THROW(platform.getCourse("non-existent"), CourseNotFoundException);
    CHECK_THROW(platform.publishCourse("non-existent"), CourseNotFoundException);
    CHECK_THROW(platform.recordStudentProgress("s-1", "c-1", 50.0), EnrollmentNotFoundException);

    // Register student and course
    Student s("s-10", "s10@edu.by", "Student Ten", 1000);
    platform.registerStudent(s);
    Instructor inst("i-10", "i10@edu.by", "Inst Ten", 1000, "Math");
    platform.registerInstructor(inst);
    Course c("c-10", "Math 101", "Basics", "i-10", "cat-math", 0.0, "Beginner");
    platform.createCourse(c);

    // Free course purchase
    Order o = platform.purchaseCourse("s-10", "c-10", "", 1000);
    CHECK(o.isPaid());
    CHECK_CLOSE(0.0, o.getFinalPrice(), 0.01);

    // Trying to purchase with invalid promo
    CHECK_THROW(platform.purchaseCourse("s-10", "c-10", "FAKEPROMO", 1000), InvalidPromoCodeException);
}

} // SUITE

int main() {
    return UnitTest::RunAllTests();
}

