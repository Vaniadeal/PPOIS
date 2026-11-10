#include "online_learning/courses/VideoLesson.hpp"

namespace online_learning {

VideoLesson::VideoLesson(std::string id, std::string moduleId, std::string title,
                         int durationMinutes, int orderIndex, bool isFreePreview,
                         std::string videoUrl, std::string resolution,
                         bool subtitles, int durationSeconds)
    : Lesson(std::move(id), std::move(moduleId), std::move(title),
             durationMinutes, orderIndex, isFreePreview),
      videoUrl_(std::move(videoUrl)), resolution_(std::move(resolution)),
      subtitlesAvailable_(subtitles), durationSeconds_(durationSeconds) {}

std::string VideoLesson::getLessonType() const {
    return "VideoLesson";
}

void VideoLesson::updateResolution(const std::string& newRes) {
    resolution_ = newRes;
}

bool VideoLesson::isHighDefinition() const {
    return resolution_ == "1080p" || resolution_ == "4K" || resolution_ == "720p";
}

std::string VideoLesson::formatDurationString() const {
    int minutes = durationSeconds_ / 60;
    int seconds = durationSeconds_ % 60;
    return std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
}

} // namespace online_learning
