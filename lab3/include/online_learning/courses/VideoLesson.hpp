#pragma once
#include <string>
#include "online_learning/courses/Lesson.hpp"

namespace online_learning {

class VideoLesson : public Lesson {
private:
    std::string videoUrl_;
    std::string resolution_;
    bool subtitlesAvailable_;
    int durationSeconds_;

public:
    VideoLesson(std::string id, std::string moduleId, std::string title,
                int durationMinutes, int orderIndex, bool isFreePreview,
                std::string videoUrl, std::string resolution,
                bool subtitles, int durationSeconds);

    std::string getLessonType() const override;
    void updateResolution(const std::string& newRes);
    bool isHighDefinition() const;
    std::string formatDurationString() const;

    const std::string& getVideoUrl() const { return videoUrl_; }
    const std::string& getResolution() const { return resolution_; }
    bool hasSubtitles() const { return subtitlesAvailable_; }
    int getDurationSeconds() const { return durationSeconds_; }
};

} // namespace online_learning
