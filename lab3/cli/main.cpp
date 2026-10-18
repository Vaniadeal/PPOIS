#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "online_learning/platform.hpp"

using namespace online_learning;

void printBanner() {
    std::cout << "=========================================================\n";
    std::cout << "       ONLINE LEARNING PLATFORM CLI (PPOIS Lab 3)        \n";
    std::cout << "=========================================================\n";
}

void printMenu() {
    std::cout << "\n--- MAIN MENU ---\n";
    std::cout << "1. Run Full End-to-End Automated Demonstration\n";
    std::cout << "2. View Course Catalog\n";
    std::cout << "3. Student Operations (Balance, Enrollment, Progress)\n";
    std::cout << "4. Quiz & Assessment Demo\n";
    std::cout << "5. Certificate Verification\n";
    std::cout << "6. Community & Support Ticket Demo\n";
    std::cout << "0. Exit\n";
    std::cout << "Select option: ";
}

void runDemonstration(OnlineLearningPlatform& platform) {
    std::cout << "\n>>> [DEMO] 1. Registering Users & Instructor...\n";
    Student student("std-101", "ivan@bsuir.by", "Ivan Kruk", 1700000000);
    student.depositFunds(150.0);
    platform.registerStudent(student);
    std::cout << "  Student registered: " << student.getFullName()
              << " | Wallet: $" << student.getWalletBalance() << "\n";

    Instructor instructor("inst-201", "andrew.ng@academy.org", "Andrew Ng", 1690000000, "Machine Learning");
    instructor.verifyInstructor();
    platform.registerInstructor(instructor);
    std::cout << "  Instructor registered: " << instructor.getFullName()
              << " | Specialization: " << instructor.getSpecialization() << "\n";

    std::cout << "\n>>> [DEMO] 2. Creating and Publishing Course...\n";
    Course course("course-cpp-ai", "C++ for Machine Learning",
                  "Comprehensive course covering C++17 modern idioms and neural inference.",
                  instructor.getId(), "cat-cs", 100.0, "Advanced");
    course.addTag("C++17");
    course.addTag("AI");
    platform.createCourse(course);
    platform.publishCourse(course.getId());
    std::cout << "  Course published: " << course.getTitle()
              << " | Price: $" << course.getPrice() << "\n";

    std::cout << "\n>>> [DEMO] 3. Applying Promo Code & Purchasing Course...\n";
    PromoCode promo("AUTUMN25", 25.0, 1800000000, 100);
    platform.addPromoCode(promo);
    Order order = platform.purchaseCourse(student.getId(), course.getId(), "AUTUMN25", 1700005000);
    std::cout << "  " << order.formatReceipt() << "\n";
    std::cout << "  Student remaining wallet: $" << platform.getStudent(student.getId()).getWalletBalance() << "\n";

    std::cout << "\n>>> [DEMO] 4. Taking Quiz & Submitting Answers...\n";
    Quiz quiz("quiz-1", "lesson-1", "Modern C++ Concepts", 15, 3, 75);
    SingleChoiceQuestion q1("q1", "quiz-1", "Which C++ standard introduced structured bindings?", 50,
                            {"C++11", "C++14", "C++17", "C++20"}, 2);
    quiz.addQuestionId("q1");

    QuizAttempt attempt("att-1", student.getId(), quiz.getQuizId(), 1700010000);
    StudentAnswer ans1("ans-1", "att-1", "q1", "2"); // Index 2 is C++17
    bool isCorrect = q1.evaluateAnswer(ans1.getChosenAnswerText());
    ans1.grade(isCorrect, q1.getPoints());
    attempt.recordAnswer(ans1);
    attempt.finalizeAttempt(100, true, 1700010600);
    std::cout << "  Question: " << q1.getText() << "\n";
    std::cout << "  Student answer result: " << ans1.formatAnswerResult() << "\n";
    std::cout << "  Quiz attempt status: " << (attempt.isPassed() ? "PASSED (100%)" : "FAILED") << "\n";

    std::cout << "\n>>> [DEMO] 5. Completing Course & Awarding Certificate...\n";
    platform.recordStudentProgress(student.getId(), course.getId(), 100.0);
    Certificate cert = platform.issueCertificate(student.getId(), course.getId(), 1700020000, "CERT-2026-999");
    std::cout << "  " << cert.formatCertificateText() << "\n";
    std::cout << "  Verification URL: " << cert.getVerificationUrl() << "\n";

    bool verified = platform.verifyCertificate(cert.getVerificationCode());
    std::cout << "  Verification check with code '" << cert.getVerificationCode()
              << "': " << (verified ? "SUCCESS (AUTHENTIC)" : "FAILED") << "\n";

    std::cout << "\n>>> [DEMO] 6. Gamification & Daily Learning Streak...\n";
    LearningStreakTracker tracker(student.getId());
    tracker.recordDailyActivity(1700000000);
    tracker.recordDailyActivity(1700086400);
    tracker.recordDailyActivity(1700172800);
    std::cout << "  " << tracker.getStreakStatusMessage() << "\n";

    std::cout << "\n>>> [DEMO] 7. Community Review & Support Ticket...\n";
    CourseReview review("rev-1", course.getId(), student.getId(), 5, "Outstanding practical material!", 1700025000, true);
    std::cout << "  " << review.formatReviewLine() << "\n";

    SupportTicket ticket("tkt-1", student.getId(), "Question about certificate print",
                         "Can I get a high-res PDF version of the certificate?", SupportTicket::Priority::Medium);
    ticket.assignToAdmin("admin-1");
    ticket.addResponse(TicketResponse("resp-1", "tkt-1", "admin-1", "A high-res vector PDF is available in your profile.", 1700026000));
    ticket.resolveTicket();
    std::cout << "  Ticket #" << ticket.getTicketId() << " status: " << ticket.getStatusString() << "\n";
    std::cout << "  Response: " << ticket.getResponses()[0].formatMessageBubble() << "\n";

    std::cout << "\n>>> [DEMO COMPLETED SUCCESSFULLY!]\n";
}

int main() {
    printBanner();
    OnlineLearningPlatform platform("NextGen Academy");

    // Initialize seed catalog
    Instructor inst("inst-1", "prof.smith@edu.org", "Dr. John Smith", 1680000000, "Computer Science");
    platform.registerInstructor(inst);
    Course c1("c-101", "Algorithms and Data Structures", "Master trees, graphs, and dynamic programming.",
              "inst-1", "cat-cs", 80.0, "Intermediate");
    Course c2("c-102", "Object-Oriented Design in C++", "SOLID principles, design patterns, and modular design.",
              "inst-1", "cat-cs", 60.0, "Intermediate");
    platform.createCourse(c1);
    platform.createCourse(c2);
    platform.publishCourse(c1.getId());
    platform.publishCourse(c2.getId());

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string discard;
            std::cin >> discard;
            continue;
        }

        switch (choice) {
            case 1:
                runDemonstration(platform);
                break;
            case 2: {
                std::cout << "\n--- PUBLISHED COURSES ---\n";
                auto list = platform.getPublishedCourses();
                for (const auto& c : list) {
                    std::cout << " * [" << c.getId() << "] " << c.getTitle()
                              << " | Level: " << c.getDifficultyLevel()
                              << " | Price: $" << c.getPrice() << "\n";
                }
                break;
            }
            case 3: {
                std::cout << "Total registered students: " << platform.getTotalStudentsCount() << "\n";
                break;
            }
            case 4: {
                Quiz q("q-demo", "l-1", "C++ Basics", 10, 2, 70);
                std::cout << "Quiz: " << q.getTitle() << ", Time limit: " << q.getTimeLimitMinutes() << " min\n";
                std::cout << "Passing percentage: " << q.getPassingPercentage() << "%\n";
                break;
            }
            case 5: {
                std::cout << "Enter verification code (or 'sample' for test): ";
                std::string code;
                std::cin >> code;
                bool ok = platform.verifyCertificate(code);
                std::cout << "Result: " << (ok ? "VALID" : "NOT FOUND / INVALID") << "\n";
                break;
            }
            case 6: {
                DiscussionForum forum("forum-1", "c-101", "Algorithms Discussion", false);
                std::cout << forum.getForumStatus() << "\n";
                break;
            }
            case 0:
                std::cout << "Exiting CLI. Goodbye!\n";
                break;
            default:
                std::cout << "Unknown choice. Try again.\n";
                break;
        }
    }

    return 0;
}

