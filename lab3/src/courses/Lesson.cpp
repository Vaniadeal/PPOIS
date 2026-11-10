#include "online_learning/courses/Lesson.hpp"

namespace online_learning {

Lesson::Lesson(std::string id, std::string moduleId, std::string title,
               int durationMinutes, int orderIndex, bool isFreePreview)
    : lessonId_(std::move(id)), moduleId_(std::move(moduleId)),
      title_(std::move(title)), durationMinutes_(durationMinutes),
      orderIndex_(orderIndex), isFreePreview_(isFreePreview) {}

void Lesson::addAttachment(const Attachment& attachment) {
    attachments_.push_back(attachment);
}

void Lesson::setFreePreview(bool freePreview) {
    isFreePreview_ = freePreview;
}

void Lesson::reorder(int newOrderIndex) {
    orderIndex_ = newOrderIndex;
}

std::string Lesson::getLessonType() const {
    return "GenericLesson";
}

} // namespace online_learning
