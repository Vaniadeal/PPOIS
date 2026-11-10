#include "online_learning/courses/Attachment.hpp"

namespace online_learning {

Attachment::Attachment()
    : attachmentId_(""), fileName_(""), fileSizeBytes_(0), fileType_(""), downloadUrl_("") {}

Attachment::Attachment(std::string id, std::string fileName, int64_t sizeBytes,
                       std::string fileType, std::string url)
    : attachmentId_(std::move(id)), fileName_(std::move(fileName)),
      fileSizeBytes_(sizeBytes), fileType_(std::move(fileType)), downloadUrl_(std::move(url)) {}

bool Attachment::isPdfDocument() const {
    return fileType_ == "pdf" || fileType_ == "application/pdf";
}

double Attachment::getFileSizeMegaBytes() const {
    constexpr double kBytesInMegaByte = 1048576.0;
    return static_cast<double>(fileSizeBytes_) / kBytesInMegaByte;
}

bool Attachment::isExceedingSizeLimit(int64_t maxBytes) const {
    return fileSizeBytes_ > maxBytes;
}

} // namespace online_learning
