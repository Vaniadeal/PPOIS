#pragma once
#include <string>
#include "online_learning/courses/Lesson.hpp"

namespace online_learning {

class TextArticleLesson : public Lesson {
private:
    std::string markdownContent_;
    int estimatedReadTimeMinutes_;
    int wordCount_;

public:
    TextArticleLesson(std::string id, std::string moduleId, std::string title,
                      int orderIndex, bool isFreePreview, std::string markdownContent);

    std::string getLessonType() const override;
    void updateContent(const std::string& newMarkdown);
    bool isShortRead() const;
    int calculateWordCount() const;

    const std::string& getMarkdownContent() const { return markdownContent_; }
    int getEstimatedReadTimeMinutes() const { return estimatedReadTimeMinutes_; }
    int getWordCount() const { return wordCount_; }
};

} // namespace online_learning
