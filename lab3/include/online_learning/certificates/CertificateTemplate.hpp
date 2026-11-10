#pragma once
#include <string>

namespace online_learning {

class CertificateTemplate {
private:
    std::string templateId_;
    std::string organizationName_;
    std::string layoutType_;
    std::string signatureImageUrl_;
    std::string borderStyle_;

public:
    CertificateTemplate();
    CertificateTemplate(std::string id, std::string orgName, std::string layout,
                        std::string sigUrl, std::string border);

    void updateOrganization(const std::string& newOrgName);
    bool isLandscapeLayout() const;
    std::string formatHeader() const;

    const std::string& getTemplateId() const { return templateId_; }
    const std::string& getOrganizationName() const { return organizationName_; }
    const std::string& getLayoutType() const { return layoutType_; }
    const std::string& getSignatureImageUrl() const { return signatureImageUrl_; }
    const std::string& getBorderStyle() const { return borderStyle_; }
};

} // namespace online_learning
