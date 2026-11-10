#pragma once
#include <string>
#include <cstdint>

namespace online_learning {

class Attachment {
private:
    std::string attachmentId_;
    std::string fileName_;
    int64_t fileSizeBytes_;
    std::string fileType_;
    std::string downloadUrl_;

public:
    Attachment();
    Attachment(std::string id, std::string fileName, int64_t sizeBytes,
               std::string fileType, std::string url);

    bool isPdfDocument() const;
    double getFileSizeMegaBytes() const;
    bool isExceedingSizeLimit(int64_t maxBytes) const;

    const std::string& getAttachmentId() const { return attachmentId_; }
    const std::string& getFileName() const { return fileName_; }
    int64_t getFileSizeBytes() const { return fileSizeBytes_; }
    const std::string& getFileType() const { return fileType_; }
    const std::string& getDownloadUrl() const { return downloadUrl_; }
};

} // namespace online_learning
