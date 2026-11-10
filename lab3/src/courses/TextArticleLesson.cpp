#include "online_learning/courses/TextArticleLesson.hpp"
#include <algorithm>

namespace online_learning {

TextArticleLesson::TextArticleLesson(std::string id, std::string moduleId, std::string title,
                                     int orderIndex, bool isFreePreview, std::string markdownContent)
    : Lesson(std::move(id), std::move(moduleId), std::move(title), 5, orderIndex, isFreePreview),
      markdownContent_(std::move(markdownContent)), estimatedReadTimeMinutes_(0), wordCount_(0) {
    wordCount_ = calculateWordCount();
    estimatedReadTimeMinutes_ = std::max(1, wordCount_ / 200);
}

std::string TextArticleLesson::getLessonType() const {
    return "TextArticleLesson";
}

void TextArticleLesson::updateContent(const std::string& newMarkdown) {
    markdownContent_ = newMarkdown;
    wordCount_ = calculateWordCount();
    estimatedReadTimeMinutes_ = std::max(1, wordCount_ / 200);
}

bool TextArticleLesson::isShortRead() const {
    return estimatedReadTimeMinutes_ <= 5;
}

int TextArticleLesson::calculateWordCount() const {
    if (markdownContent_.empty()) {
        return 0;
    }
    int count = 1;
    for (char character : markdownContent_) {
        if (character == ' ' || character == '\n') {
            count++;
        }
    }
    return count;
}

} // namespace online_learning
