#pragma once
#include <string>
#include "online_learning/courses/Lesson.hpp"

namespace online_learning {

class InteractiveCodingLesson : public Lesson {
private:
    std::string initialCode_;
    std::string programmingLanguage_;
    std::string testScript_;
    int timeLimitSeconds_;

public:
    InteractiveCodingLesson(std::string id, std::string moduleId, std::string title,
                            int durationMinutes, int orderIndex, bool isFreePreview,
                            std::string initialCode, std::string language,
                            std::string testScript, int timeLimitSeconds);

    std::string getLessonType() const override;
    bool isLanguageSupported(const std::string& expectedLanguage) const;
    void updateTestScript(const std::string& newScript);
    bool validateTimeLimit() const;

    const std::string& getInitialCode() const { return initialCode_; }
    const std::string& getProgrammingLanguage() const { return programmingLanguage_; }
    const std::string& getTestScript() const { return testScript_; }
    int getTimeLimitSeconds() const { return timeLimitSeconds_; }
};

} // namespace online_learning
