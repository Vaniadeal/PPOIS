#include "online_learning/certificates/CertificateTemplate.hpp"

namespace online_learning {

CertificateTemplate::CertificateTemplate()
    : templateId_(""), organizationName_("Online Learning Academy"),
      layoutType_("Landscape"), signatureImageUrl_("sig.png"), borderStyle_("ClassicGold") {}

CertificateTemplate::CertificateTemplate(std::string id, std::string orgName, std::string layout,
                                         std::string sigUrl, std::string border)
    : templateId_(std::move(id)), organizationName_(std::move(orgName)),
      layoutType_(std::move(layout)), signatureImageUrl_(std::move(sigUrl)),
      borderStyle_(std::move(border)) {}

void CertificateTemplate::updateOrganization(const std::string& newOrgName) {
    organizationName_ = newOrgName;
}

bool CertificateTemplate::isLandscapeLayout() const {
    return layoutType_ == "Landscape";
}

std::string CertificateTemplate::formatHeader() const {
    return organizationName_ + " [" + layoutType_ + ", " + borderStyle_ + "]";
}

} // namespace online_learning
