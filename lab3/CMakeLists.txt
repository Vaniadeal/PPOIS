cmake_minimum_required(VERSION 3.16)
project(lab3_online_learning CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

option(ENABLE_COVERAGE "Enable code coverage flags" OFF)

# Domain library with individual class implementation files
add_library(online_learning_lib
    # Users
    src/users/UserProfile.cpp
    src/users/UserCredentials.cpp
    src/users/User.cpp
    src/users/Student.cpp
    src/users/Instructor.cpp
    src/users/TeachingAssistant.cpp
    src/users/Administrator.cpp
    src/users/RoleManager.cpp
    src/users/Session.cpp
    src/users/AuthenticationService.cpp

    # Courses
    src/courses/Tag.cpp
    src/courses/CourseCategory.cpp
    src/courses/Attachment.cpp
    src/courses/Lesson.cpp
    src/courses/VideoLesson.cpp
    src/courses/TextArticleLesson.cpp
    src/courses/InteractiveCodingLesson.cpp
    src/courses/CourseModule.cpp
    src/courses/CourseSyllabus.cpp
    src/courses/Course.cpp

    # Assessments
    src/assessments/Question.cpp
    src/assessments/SingleChoiceQuestion.cpp
    src/assessments/MultipleChoiceQuestion.cpp
    src/assessments/CodingQuestion.cpp
    src/assessments/StudentAnswer.cpp
    src/assessments/QuizAttempt.cpp
    src/assessments/Quiz.cpp
    src/assessments/HomeworkAssignment.cpp
    src/assessments/HomeworkSubmission.cpp
    src/assessments/HomeworkReview.cpp

    # Progress
    src/progress/LessonProgress.cpp
    src/progress/Enrollment.cpp
    src/progress/StudentGrade.cpp
    src/progress/GradeBook.cpp
    src/progress/LearningStreakTracker.cpp
    src/progress/CourseAnalytics.cpp
    src/progress/EngagementMetric.cpp

    # Community
    src/community/CourseReview.cpp
    src/community/ForumPost.cpp
    src/community/ForumThread.cpp
    src/community/DiscussionForum.cpp
    src/community/TicketResponse.cpp
    src/community/SupportTicket.cpp

    # Finance
    src/finance/PaymentTransaction.cpp
    src/finance/PromoCode.cpp
    src/finance/Order.cpp
    src/finance/SubscriptionPlan.cpp
    src/finance/UserSubscription.cpp
    src/finance/RefundRequest.cpp
    src/finance/PayoutRecord.cpp

    # Certificates
    src/certificates/CertificateTemplate.cpp
    src/certificates/Certificate.cpp
    src/certificates/AchievementBadge.cpp
    src/certificates/StudentBadgeAward.cpp

    # Platform
    src/platform/Notification.cpp
    src/platform/NotificationService.cpp
    src/platform/OnlineLearningPlatform.cpp
)
target_include_directories(online_learning_lib PUBLIC include)

# Coverage options
if(ENABLE_COVERAGE)
    target_compile_options(online_learning_lib PUBLIC --coverage -O0 -g)
    target_link_options(online_learning_lib PUBLIC --coverage)
endif()

# CLI Application
add_executable(learning_platform_app cli/main.cpp)
target_link_libraries(learning_platform_app PRIVATE online_learning_lib)

# Testing via UnitTest++
include(FetchContent)
FetchContent_Declare(
    UnitTestPlusPlus
    GIT_REPOSITORY https://github.com/unittest-cpp/unittest-cpp.git
    GIT_TAG        v2.0.0
)
set(UTPP_USE_PLUS_PLUS_17 ON CACHE BOOL "" FORCE)
set(UTPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(UTPP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(UnitTestPlusPlus)

if(TARGET TestUnitTest++)
    target_compile_options(TestUnitTest++ PRIVATE -Wno-error)
endif()

enable_testing()
add_executable(learning_platform_tests tests/test_online_learning.cpp)
target_link_libraries(learning_platform_tests PRIVATE online_learning_lib UnitTest++)
target_include_directories(learning_platform_tests PRIVATE
    ${unittestplusplus_SOURCE_DIR}
    ${unittestplusplus_SOURCE_DIR}/UnitTest++
)
add_test(NAME domain_tests COMMAND learning_platform_tests)
