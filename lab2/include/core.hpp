#pragma once
#include "exceptions.hpp"
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <chrono>
#include <cmath>

namespace pharma {

using Date = std::chrono::system_clock::time_point;

Date makeDate(int y, int m, int d);
bool isExpired(const Date& d);

class Address {
    std::string street_;
    std::string city_;
    std::string postalCode_;
    std::string country_;
    double latitude_;
    double longitude_;
public:
    Address(std::string street, std::string city, std::string postal,
            std::string country, double lat, double lon);
    bool validate() const;
    std::string formatFull() const;
    double distanceTo(const Address& other) const;
    bool isInCountry(const std::string& country) const;
    void normalize();
    const std::string& city() const { return city_; }
    const std::string& country() const { return country_; }
};

class ContactInfo {
    std::string phone_;
    std::string email_;
    std::string fax_;
    std::string website_;
public:
    ContactInfo(std::string phone, std::string email, std::string fax = "",
                std::string site = "");
    bool validate() const;
    std::string maskPhone() const;
    std::string format() const;
    const std::string& email() const { return email_; }
};

class Category {
    std::string id_;
    std::string name_;
    std::string description_;
    std::string parentId_;
public:
    Category(std::string id, std::string name, std::string desc = "",
             std::string parent = "");
    bool isRoot() const;
    std::string getFullPath(const std::vector<Category>& all) const;
    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
};

class Medicine {
protected:
    std::string id_;
    std::string name_;
    std::string description_;
    double price_;
    int quantity_;
    Date expiryDate_;
    std::string manufacturerId_;
    std::string categoryId_;
    double dosage_;
    std::string form_;
    bool prescriptionRequired_;
    std::string barcode_;
public:
    Medicine(std::string id, std::string name, double price, int qty,
             Date exp, std::string manId, std::string catId, double dose,
             std::string form, bool rx, std::string barcode);
    virtual ~Medicine() = default;

    bool isExpired() const;
    double calculateDiscount(double percent) const;
    virtual bool needsPrescription() const;
    void updatePrice(double newPrice);
    std::string getFullInfo() const;

    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
    double price() const { return price_; }
    int quantity() const { return quantity_; }
    void setQuantity(int q) { quantity_ = q; }
    const Date& expiryDate() const { return expiryDate_; }
};

class PrescriptionMedicine : public Medicine {
    std::string prescriptionForm_;
    double maxDose_;
public:
    PrescriptionMedicine(std::string id, std::string name, double price,
                         int qty, Date exp, std::string manId,
                         std::string catId, double dose, std::string form,
                         std::string barcode, std::string rxForm, double maxDose);
    bool requirePrescription() const;
    bool validateDose(double dose) const;
    bool needsPrescription() const override { return true; }
};

class OverTheCounterMedicine : public Medicine {
    int maxQuantityPerSale_;
    int ageRestriction_;
public:
    OverTheCounterMedicine(std::string id, std::string name, double price,
                           int qty, Date exp, std::string manId,
                           std::string catId, double dose, std::string form,
                           std::string barcode, int maxQty, int age);
    bool canSellWithoutPrescription() const;
    bool checkAgeLimit(int age) const;
    bool needsPrescription() const override { return false; }
};

class Vaccine : public PrescriptionMedicine {
    double storageTemperature_;
    int dosesPerVial_;
    std::string vaccineType_;
public:
    Vaccine(std::string id, std::string name, double price, int qty, Date exp,
            std::string manId, std::string catId, std::string barcode,
            std::string rxForm, double maxDose, double st, int dpv,
            std::string type);
    bool requiresColdChain() const { return storageTemperature_ < 8.0; }
    bool checkTemperature(double t) const;
};

class Antibiotic : public PrescriptionMedicine {
    std::string spectrum_;
    double resistanceRisk_;
public:
    Antibiotic(std::string id, std::string name, double price, int qty,
               Date exp, std::string manId, std::string catId,
               std::string barcode, std::string rxForm, double maxDose,
               std::string spectrum, double risk);
    bool isBroadSpectrum() const { return spectrum_ == "broad"; }
    double assessResistance() const { return resistanceRisk_; }
};

class Analgesic : public OverTheCounterMedicine {
    std::string painType_;
    std::string strength_;
public:
    Analgesic(std::string id, std::string name, double price, int qty, Date exp,
              std::string manId, std::string catId, std::string barcode,
              int maxQty, int age, std::string painType, std::string strength);
    bool isStrong() const { return strength_ == "strong"; }
    bool recommendForPain(const std::string& type) const;
};

class License {
    std::string number_;
    Date issued_;
    Date validUntil_;
public:
    License(std::string num, Date issued, Date validUntil);
    bool isValid() const;
    bool isExpired() const;
    const std::string& number() const { return number_; }
};

class Manufacturer {
    std::string id_;
    std::string name_;
    std::string country_;
    std::string contactId_;
    std::string licenseId_;
    double rating_;
public:
    Manufacturer(std::string id, std::string name, std::string country,
                 std::string contactId, std::string licenseId, double rating);
    bool validateLicense(const License& l) const;
    double calculateRating(int feedbackCount, double sum) const;
    bool isReliable() const { return rating_ >= 4.0; }
    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
};

class SupplierRating {
    std::string supplierId_;
    double averageScore_;
    int reviewsCount_;
    std::string lastComment_;
public:
    SupplierRating(std::string sid, double avg, int reviews,
                   std::string comment = "");
    double score() const { return averageScore_; }
    bool isTopRated() const { return averageScore_ >= 4.5; }
};

class Supplier {
    std::string id_;
    std::string name_;
    std::string country_;
    std::string contactId_;
    SupplierRating rating_;
    double minOrderAmount_;
public:
    Supplier(std::string id, std::string name, std::string country,
             std::string contactId, SupplierRating rating, double minOrder);
    bool canFulfill(double amount) const;
    double calculateShippingCost(double amount) const;
    bool isPreferred() const;
    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
};

class Certificate {
    std::string number_;
    Date issued_;
    Date validUntil_;
public:
    Certificate(std::string num, Date issued, Date validUntil);
    bool isValid() const;
    bool matchesMedicine(const std::string& medId) const;
};

class Customer {
protected:
    std::string id_;
    std::string name_;
    Address address_;
    ContactInfo contact_;
    double balance_;
    std::string inn_;
    std::string type_;
public:
    Customer(std::string id, std::string name, Address addr,
             ContactInfo contact, double balance, std::string inn,
             std::string type);
    virtual ~Customer() = default;

    bool hasDebt() const;
    void topUp(double amount);
    bool withdraw(double amount);
    virtual double calculateDiscount() const;
    std::string getShortInfo() const;

    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
    double balance() const { return balance_; }
};

class Pharmacy : public Customer {
public:
    Pharmacy(std::string id, std::string name, Address addr,
             ContactInfo contact, double balance, std::string inn);
    bool hasValidLicense(const License& l) const;
    double calculateDiscount() const override;
    bool canOrderPrescription() const;
};

class Hospital : public Customer {
    int beds_;
public:
    Hospital(std::string id, std::string name, Address addr,
             ContactInfo contact, double balance, std::string inn, int beds);
    double calculateDiscount() const override;
    bool isLarge() const { return beds_ >= 300; }
    bool needsEmergencySupply() const;
};

class Clinic : public Customer {
    std::string specialization_;
public:
    Clinic(std::string id, std::string name, Address addr, ContactInfo contact,
           double balance, std::string inn, std::string spec);
    double calculateDiscount() const override;
    bool isSpecialized() const;
};

}