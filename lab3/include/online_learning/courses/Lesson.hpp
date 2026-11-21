#pragma once
#include <string>
#include <vector>
#include "online_learning/courses/Attachment.hpp"

namespace online_learning {

class Lesson {
protected:
    std::string lessonId_;
    std::string moduleId_;
    std::string title_;
    int durationMinutes_;
    int orderIndex_;
    bool isFreePreview_;
    std::vector<Attachment> attachments_;

public:
    Lesson(std::string id, std::string moduleId, std::string title,
           int durationMinutes, int orderIndex, bool isFreePreview);
    virtual ~Lesson() = default;

    void addAttachment(const Attachment& attachment);
    void setFreePreview(bool freePreview);
    void reorder(int newOrderIndex);
    virtual std::string getLessonType() const;

    const std::string& getLessonId() const { return lessonId_; }
    const std::string& getModuleId() const { return moduleId_; }
    const std::string& getTitle() const { return title_; }
    int getDurationMinutes() const { return durationMinutes_; }
    int getOrderIndex() const { return orderIndex_; }
    bool isFreePreview() const { return isFreePreview_; }
    const std::vector<Attachment>& getAttachments() const { return attachments_; }
};

} // namespace online_learning
