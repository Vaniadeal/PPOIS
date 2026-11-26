#include "core.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace pharma {

Date makeDate(int y, int m, int d) {
    std::tm tm{};
    tm.tm_year = y - 1900; tm.tm_mon = m - 1; tm.tm_mday = d;
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

bool isExpired(const Date& d) {
    return d < std::chrono::system_clock::now();
}

Address::Address(std::string street, std::string city, std::string postal,
                 std::string country, double lat, double lon)
    : street_(std::move(street)), city_(std::move(city)),
      postalCode_(std::move(postal)), country_(std::move(country)),
      latitude_(lat), longitude_(lon) {}

bool Address::validate() const {
    return !street_.empty() && !city_.empty() && !country_.empty() &&
           latitude_ >= -90.0 && latitude_ <= 90.0 &&
           longitude_ >= -180.0 && longitude_ <= 180.0;
}

std::string Address::formatFull() const {
    std::ostringstream os;
    os << country_ << ", " << city_ << ", " << street_ << ", " << postalCode_;
    return os.str();
}

double Address::distanceTo(const Address& other) const {
    constexpr double R = 6371.0;
    constexpr double PI = 3.14159265358979323846;
    double dLat = (other.latitude_ - latitude_) * PI / 180.0;
    double dLon = (other.longitude_ - longitude_) * PI / 180.0;
    double a = std::sin(dLat/2) * std::sin(dLat/2) +
               std::cos(latitude_*PI/180.0) * std::cos(other.latitude_*PI/180.0) *
               std::sin(dLon/2) * std::sin(dLon/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    return R * c;
}

bool Address::isInCountry(const std::string& country) const {
    return country_ == country;
}

void Address::normalize() {
    std::transform(city_.begin(), city_.end(), city_.begin(), ::toupper);
}

ContactInfo::ContactInfo(std::string phone, std::string email, std::string fax,
                         std::string site)
    : phone_(std::move(phone)), email_(std::move(email)),
      fax_(std::move(fax)), website_(std::move(site)) {}

bool ContactInfo::validate() const {
    auto at = email_.find('@');
    return !phone_.empty() && at != std::string::npos &&
           at > 0 && at + 1 < email_.size();
}

std::string ContactInfo::maskPhone() const {
    if (phone_.size() < 4) return "***";
    return std::string(phone_.size() - 4, '*') + phone_.substr(phone_.size() - 4);
}

std::string ContactInfo::format() const { return phone_ + " | " + email_; }

Category::Category(std::string id, std::string name, std::string desc,
                   std::string parent)
    : id_(std::move(id)), name_(std::move(name)),
      description_(std::move(desc)), parentId_(std::move(parent)) {}

bool Category::isRoot() const { return parentId_.empty(); }

std::string Category::getFullPath(const std::vector<Category>& all) const {
    std::string path = name_;
    std::string pid = parentId_;
    while (!pid.empty()) {
        auto it = std::find_if(all.begin(), all.end(),
            [&](const Category& c){ return c.id() == pid; });
        if (it == all.end()) break;
        path = it->name() + " / " + path;
        pid = it->parentId_;
    }
    return path;
}

Medicine::Medicine(std::string id, std::string name, double price, int qty,
                   Date exp, std::string manId, std::string catId, double dose,
                   std::string form, bool rx, std::string barcode)
    : id_(std::move(id)), name_(std::move(name)), price_(price),
      quantity_(qty), expiryDate_(exp), manufacturerId_(std::move(manId)),
      categoryId_(std::move(catId)), dosage_(dose), form_(std::move(form)),
      prescriptionRequired_(rx), barcode_(std::move(barcode)) {
    if (price_ < 0) throw InvalidMedicineDataException("negative price");
}

bool Medicine::isExpired() const { return pharma::isExpired(expiryDate_); }

double Medicine::calculateDiscount(double percent) const {
    return price_ * (1.0 - percent / 100.0);
}

bool Medicine::needsPrescription() const { return prescriptionRequired_; }

void Medicine::updatePrice(double newPrice) {
    if (newPrice < 0) throw InvalidMedicineDataException("negative price");
    price_ = newPrice;
}

std::string Medicine::getFullInfo() const {
    std::ostringstream os;
    os << id_ << " " << name_ << " price=" << price_ << " qty=" << quantity_;
    return os.str();
}

PrescriptionMedicine::PrescriptionMedicine(
    std::string id, std::string name, double price, int qty, Date exp,
    std::string manId, std::string catId, double dose, std::string form,
    std::string barcode, std::string rxForm, double maxDose)
    : Medicine(std::move(id), std::move(name), price, qty, exp, std::move(manId),
               std::move(catId), dose, std::move(form), true, std::move(barcode)),
      prescriptionForm_(std::move(rxForm)), maxDose_(maxDose) {}

bool PrescriptionMedicine::requirePrescription() const { return true; }
bool PrescriptionMedicine::validateDose(double dose) const {
    return dose > 0 && dose <= maxDose_;
}

OverTheCounterMedicine::OverTheCounterMedicine(
    std::string id, std::string name, double price, int qty, Date exp,
    std::string manId, std::string catId, double dose, std::string form,
    std::string barcode, int maxQty, int age)
    : Medicine(std::move(id), std::move(name), price, qty, exp, std::move(manId),
               std::move(catId), dose, std::move(form), false, std::move(barcode)),
      maxQuantityPerSale_(maxQty), ageRestriction_(age) {}

bool OverTheCounterMedicine::canSellWithoutPrescription() const { return true; }
bool OverTheCounterMedicine::checkAgeLimit(int age) const {
    return age >= ageRestriction_;
}

Vaccine::Vaccine(std::string id, std::string name, double price, int qty,
                 Date exp, std::string manId, std::string catId,
                 std::string barcode, std::string rxForm, double maxDose,
                 double st, int dpv, std::string type)
    : PrescriptionMedicine(std::move(id), std::move(name), price, qty, exp,
        std::move(manId), std::move(catId), 0.5, "injection", std::move(barcode),
        std::move(rxForm), maxDose),
      storageTemperature_(st), dosesPerVial_(dpv), vaccineType_(std::move(type)) {}

bool Vaccine::checkTemperature(double t) const {
    if (requiresColdChain() && t > 8.0)
        throw ColdChainViolationException(t, 2.0, 8.0);
    return true;
}

Antibiotic::Antibiotic(std::string id, std::string name, double price, int qty,
                       Date exp, std::string manId, std::string catId,
                       std::string barcode, std::string rxForm, double maxDose,
                       std::string spectrum, double risk)
    : PrescriptionMedicine(std::move(id), std::move(name), price, qty, exp,
        std::move(manId), std::move(catId), 1.0, "tablet", std::move(barcode),
        std::move(rxForm), maxDose),
      spectrum_(std::move(spectrum)), resistanceRisk_(risk) {}

Analgesic::Analgesic(std::string id, std::string name, double price, int qty,
                     Date exp, std::string manId, std::string catId,
                     std::string barcode, int maxQty, int age,
                     std::string painType, std::string strength)
    : OverTheCounterMedicine(std::move(id), std::move(name), price, qty, exp,
        std::move(manId), std::move(catId), 0.5, "tablet", std::move(barcode),
        maxQty, age),
      painType_(std::move(painType)), strength_(std::move(strength)) {}

bool Analgesic::recommendForPain(const std::string& type) const {
    return painType_ == type;
}

License::License(std::string num, Date issued, Date validUntil)
    : number_(std::move(num)), issued_(issued), validUntil_(validUntil) {}

bool License::isValid() const { return !isExpired(); }
bool License::isExpired() const {
    return validUntil_ < std::chrono::system_clock::now();
}

Manufacturer::Manufacturer(std::string id, std::string name, std::string country,
                           std::string contactId, std::string licenseId,
                           double rating)
    : id_(std::move(id)), name_(std::move(name)), country_(std::move(country)),
      contactId_(std::move(contactId)), licenseId_(std::move(licenseId)),
      rating_(rating) {}

bool Manufacturer::validateLicense(const License& l) const {
    if (!l.isValid()) throw InvalidLicenseException(l.number());
    return true;
}
double Manufacturer::calculateRating(int feedbackCount, double sum) const {
    return feedbackCount > 0 ? sum / feedbackCount : 0.0;
}

SupplierRating::SupplierRating(std::string sid, double avg, int reviews,
                               std::string comment)
    : supplierId_(std::move(sid)), averageScore_(avg), reviewsCount_(reviews),
      lastComment_(std::move(comment)) {}

Supplier::Supplier(std::string id, std::string name, std::string country,
                   std::string contactId, SupplierRating rating, double minOrder)
    : id_(std::move(id)), name_(std::move(name)), country_(std::move(country)),
      contactId_(std::move(contactId)), rating_(std::move(rating)),
      minOrderAmount_(minOrder) {}

bool Supplier::canFulfill(double amount) const { return amount >= minOrderAmount_; }
double Supplier::calculateShippingCost(double amount) const {
    constexpr double FREE_THRESHOLD = 100000.0;
    constexpr double SHIPPING_RATE = 0.02;
    if (amount > FREE_THRESHOLD) return 0.0;
    return amount * SHIPPING_RATE;
}
bool Supplier::isPreferred() const { return rating_.isTopRated(); }

Certificate::Certificate(std::string num, Date issued, Date validUntil)
    : number_(std::move(num)), issued_(issued), validUntil_(validUntil) {}

bool Certificate::isValid() const {
    return validUntil_ > std::chrono::system_clock::now();
}
bool Certificate::matchesMedicine(const std::string& medId) const {
    return number_.find(medId) != std::string::npos;
}

Customer::Customer(std::string id, std::string name, Address addr,
                   ContactInfo contact, double balance, std::string inn,
                   std::string type)
    : id_(std::move(id)), name_(std::move(name)), address_(std::move(addr)),
      contact_(std::move(contact)), balance_(balance), inn_(std::move(inn)),
      type_(std::move(type)) {}

bool Customer::hasDebt() const { return balance_ < 0; }
void Customer::topUp(double amount) { if (amount > 0) balance_ += amount; }
bool Customer::withdraw(double amount) {
    if (amount <= 0) throw PaymentFailedException("non-positive amount");
    if (balance_ < amount) throw InsufficientFundsException(amount, balance_);
    balance_ -= amount;
    return true;
}
double Customer::calculateDiscount() const { return 0.0; }
std::string Customer::getShortInfo() const { return id_ + " " + name_; }

Pharmacy::Pharmacy(std::string id, std::string name, Address addr,
                   ContactInfo contact, double balance, std::string inn)
    : Customer(std::move(id), std::move(name), std::move(addr),
               std::move(contact), balance, std::move(inn), "pharmacy") {}

bool Pharmacy::hasValidLicense(const License& l) const { return l.isValid(); }
double Pharmacy::calculateDiscount() const { return 5.0; }
bool Pharmacy::canOrderPrescription() const { return !hasDebt(); }

Hospital::Hospital(std::string id, std::string name, Address addr,
                   ContactInfo contact, double balance, std::string inn, int beds)
    : Customer(std::move(id), std::move(name), std::move(addr),
               std::move(contact), balance, std::move(inn), "hospital"),
      beds_(beds) {}

double Hospital::calculateDiscount() const { return isLarge() ? 12.0 : 7.0; }
bool Hospital::needsEmergencySupply() const { return beds_ > 0; }

Clinic::Clinic(std::string id, std::string name, Address addr, ContactInfo contact,
               double balance, std::string inn, std::string spec)
    : Customer(std::move(id), std::move(name), std::move(addr),
               std::move(contact), balance, std::move(inn), "clinic"),
      specialization_(std::move(spec)) {}

double Clinic::calculateDiscount() const { return 3.0; }
bool Clinic::isSpecialized() const { return !specialization_.empty(); }

} 