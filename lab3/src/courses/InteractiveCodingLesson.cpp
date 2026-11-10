#include "online_learning/courses/InteractiveCodingLesson.hpp"

namespace online_learning {

InteractiveCodingLesson::InteractiveCodingLesson(std::string id, std::string moduleId, std::string title,
                                                 int durationMinutes, int orderIndex, bool isFreePreview,
                                                 std::string initialCode, std::string language,
                                                 std::string testScript, int timeLimitSeconds)
    : Lesson(std::move(id), std::move(moduleId), std::move(title), durationMinutes, orderIndex, isFreePreview),
      initialCode_(std::move(initialCode)), programmingLanguage_(std::move(language)),
      testScript_(std::move(testScript)), timeLimitSeconds_(timeLimitSeconds) {}

std::string InteractiveCodingLesson::getLessonType() const {
    return "InteractiveCodingLesson";
}

bool InteractiveCodingLesson::isLanguageSupported(const std::string& expectedLanguage) const {
    return programmingLanguage_ == expectedLanguage;
}

void InteractiveCodingLesson::updateTestScript(const std::string& newScript) {
    testScript_ = newScript;
}

bool InteractiveCodingLesson::validateTimeLimit() const {
    return timeLimitSeconds_ > 0 && timeLimitSeconds_ <= 600;
}

} // namespace online_learning
