#include "UnitTest++/UnitTest++.h"

#include "core.hpp"
#include "orders.hpp"
#include "warehouse.hpp"
#include "employees.hpp"
#include "transport.hpp"
#include "documents.hpp"
#include "reports.hpp"
#include "security.hpp"
#include "quality.hpp"

#include <chrono>

using namespace pharma;

// ─────────── helpers ───────────
namespace {
Date futureDate() { return makeDate(2030, 1, 1); }
Date pastDate()   { return makeDate(2020, 1, 1); }

Address addrA() { return Address("A", "Minsk", "220000", "BY", 53.9, 27.5); }
Address addrB() { return Address("B", "Minsk", "220000", "BY", 53.95, 27.55); }
ContactInfo contact() { return ContactInfo("+375291234567", "u@m.com", "fax", "site"); }
}

// ═══════════════════════ core.cpp ═══════════════════════

TEST(Address_ValidateAndFormat) {
    Address a("A", "Minsk", "220000", "BY", 53.9, 27.5);
    CHECK(a.validate());
    CHECK(a.formatFull().find("Minsk") != std::string::npos);
    CHECK(a.isInCountry("BY"));
    CHECK(!a.isInCountry("PL"));

    Address bad("", "", "", "", 200, 300);
    CHECK(!bad.validate());
}

TEST(Address_DistanceAndNormalize) {
    Address a = addrA();
    Address b = addrB();
    CHECK(a.distanceTo(b) > 0.0);
    CHECK(a.distanceTo(a) < 0.001);
    a.normalize();
    CHECK_EQUAL(std::string("MINSK"), a.city());
}

TEST(ContactInfo_ValidateMaskFormat) {
    ContactInfo c("+375291234567", "u@m.com");
    CHECK(c.validate());
    CHECK_EQUAL(std::string("4567"), c.maskPhone().substr(c.maskPhone().size() - 4));
    CHECK(c.format().find("u@m.com") != std::string::npos);
    CHECK_EQUAL(std::string("u@m.com"), c.email());

    ContactInfo tiny("12", "u@m.com");
    CHECK_EQUAL(std::string("***"), tiny.maskPhone());

    ContactInfo bad("", "no-at-sign");
    CHECK(!bad.validate());
}

TEST(Category_RootAndFullPath) {
    Category root("C1", "Root");
    CHECK(root.isRoot());
    CHECK_EQUAL(std::string("C1"), root.id());
    CHECK_EQUAL(std::string("Root"), root.name());

    std::vector<Category> all;
    all.push_back(Category("C1", "Root"));
    all.push_back(Category("C2", "Child", "", "C1"));
    all.push_back(Category("C3", "Leaf", "", "C2"));
    std::string path = all[2].getFullPath(all);
    CHECK(path.find("Root") != std::string::npos);
    CHECK(path.find("Child") != std::string::npos);
    CHECK(path.find("Leaf") != std::string::npos);
}

TEST(Medicine_BasicAndDiscount) {
    Medicine m("M1", "Med", 100.0, 100, futureDate(), "MAN", "CAT", 1.0, "tab", false, "BC");
    CHECK(!m.isExpired());
    CHECK_EQUAL(std::string("M1"), m.id());
    CHECK_EQUAL(std::string("Med"), m.name());
    CHECK_CLOSE(100.0, m.price(), 0.001);
    CHECK_EQUAL(100, m.quantity());
    CHECK(!m.needsPrescription());
    CHECK(m.getFullInfo().find("M1") != std::string::npos);
    CHECK_CLOSE(90.0, m.calculateDiscount(10.0), 0.001);

    m.updatePrice(150.0);
    CHECK_CLOSE(150.0, m.price(), 0.001);
    CHECK_THROW(m.updatePrice(-1.0), InvalidMedicineDataException);

    m.setQuantity(50);
    CHECK_EQUAL(50, m.quantity());
}

TEST(Medicine_Expired) {
    Medicine m("M1", "Med", 10.0, 100, pastDate(), "MAN", "CAT", 1.0, "tab", false, "BC");
    CHECK(m.isExpired());
}

TEST(Medicine_NegativePriceThrows) {
    CHECK_THROW(
        Medicine("X","Y",-1.0,1,futureDate(),"M","C",1.0,"t",false,"b"),
        InvalidMedicineDataException);
}

TEST(PrescriptionMedicine_RequiresRx) {
    PrescriptionMedicine pm("P1", "Rx", 20.0, 50, futureDate(), "M", "C",
                            1.0, "tab", "BC", "form", 5.0);
    CHECK(pm.needsPrescription());
    CHECK(pm.requirePrescription());
    CHECK(pm.validateDose(2.0));
    CHECK(!pm.validateDose(10.0));
    CHECK(!pm.validateDose(-1.0));
}

TEST(OverTheCounterMedicine) {
    OverTheCounterMedicine o("O1", "OTC", 5.0, 100, futureDate(), "M", "C",
                             1.0, "tab", "BC", 5, 12);
    CHECK(!o.needsPrescription());
    CHECK(o.canSellWithoutPrescription());
    CHECK(o.checkAgeLimit(18));
    CHECK(!o.checkAgeLimit(10));
}

TEST(Vaccine_ColdChain) {
    Vaccine v("V1", "V", 10.0, 100, futureDate(), "M", "C", "BC",
              "form", 1.0, 4.0, 5, "mRNA");
    CHECK(v.requiresColdChain());
    CHECK(v.checkTemperature(5.0));
    CHECK_THROW(v.checkTemperature(20.0), ColdChainViolationException);

    Vaccine warm("V2", "V2", 10.0, 100, futureDate(), "M", "C", "BC",
                 "form", 1.0, 15.0, 5, "type");
    CHECK(!warm.requiresColdChain());
    CHECK(warm.checkTemperature(20.0));
}

TEST(Antibiotic_Spectrum) {
    Antibiotic a("A1", "A", 20.0, 50, futureDate(), "M", "C", "BC",
                 "form", 2.0, "broad", 0.5);
    CHECK(a.isBroadSpectrum());
    CHECK_CLOSE(0.5, a.assessResistance(), 0.001);

    Antibiotic n("A2", "A2", 20.0, 50, futureDate(), "M", "C", "BC",
                 "form", 2.0, "narrow", 0.1);
    CHECK(!n.isBroadSpectrum());
}

TEST(Analgesic) {
    Analgesic m("M1", "P", 5.0, 100, futureDate(), "M", "C", "BC",
                5, 12, "headache", "strong");
    CHECK(m.isStrong());
    CHECK(m.recommendForPain("headache"));
    CHECK(!m.recommendForPain("back"));

    Analgesic mild("M2", "P2", 5.0, 100, futureDate(), "M", "C", "BC",
                   5, 12, "mild", "mild");
    CHECK(!mild.isStrong());
}

TEST(License_ValidAndExpired) {
    License l("L1", pastDate(), futureDate());
    CHECK(l.isValid());
    CHECK(!l.isExpired());
    CHECK_EQUAL(std::string("L1"), l.number());

    License expired("L2", pastDate(), pastDate());
    CHECK(!expired.isValid());
    CHECK(expired.isExpired());
}

TEST(Manufacturer) {
    Manufacturer m("M1", "Name", "BY", "C1", "L1", 4.5);
    CHECK(m.isReliable());
    CHECK_EQUAL(std::string("M1"), m.id());
    CHECK_EQUAL(std::string("Name"), m.name());

    License valid("L1", pastDate(), futureDate());
    CHECK(m.validateLicense(valid));
    License invalid("L2", pastDate(), pastDate());
    CHECK_THROW(m.validateLicense(invalid), InvalidLicenseException);

    CHECK_CLOSE(4.0, m.calculateRating(10, 40.0), 0.001);
    CHECK_CLOSE(0.0, m.calculateRating(0, 0.0), 0.001);
}

TEST(SupplierRating_TopRated) {
    SupplierRating r("S1", 4.7, 10, "great");
    CHECK_CLOSE(4.7, r.score(), 0.001);
    CHECK(r.isTopRated());

    SupplierRating low("S2", 3.0, 5, "");
    CHECK(!low.isTopRated());
}

TEST(Supplier) {
    SupplierRating r("S1", 4.7, 10, "");
    Supplier s("S1", "Sup", "BY", "C1", r, 1000.0);
    CHECK(s.canFulfill(2000.0));
    CHECK(!s.canFulfill(500.0));
    CHECK(s.isPreferred());
    CHECK_CLOSE(0.0, s.calculateShippingCost(200000.0), 0.001);
    CHECK_CLOSE(20.0, s.calculateShippingCost(1000.0), 0.001);
    CHECK_EQUAL(std::string("S1"), s.id());
    CHECK_EQUAL(std::string("Sup"), s.name());
}

TEST(Certificate) {
    Certificate c("CERT-M1", pastDate(), futureDate());
    CHECK(c.isValid());
    CHECK(c.matchesMedicine("M1"));
    CHECK(!c.matchesMedicine("M9"));

    Certificate expired("C2", pastDate(), pastDate());
    CHECK(!expired.isValid());
}

TEST(Customer_BaseOperations) {
    Customer c("C1", "Cust", addrA(), contact(), 100.0, "INN", "type");
    CHECK(!c.hasDebt());
    c.topUp(50.0);
    CHECK_CLOSE(150.0, c.balance(), 0.001);
    CHECK(c.withdraw(30.0));
    CHECK_CLOSE(120.0, c.balance(), 0.001);
    CHECK(c.getShortInfo().find("C1") != std::string::npos);
    CHECK_CLOSE(0.0, c.calculateDiscount(), 0.001);
}

TEST(Customer_DebtAndErrors) {
    Customer c("C1", "Cust", addrA(), contact(), -50.0, "INN", "type");
    CHECK(c.hasDebt());
    CHECK_THROW(c.withdraw(-1.0), PaymentFailedException);
    CHECK_THROW(c.withdraw(1000.0), InsufficientFundsException);
    c.topUp(-10.0);
    CHECK_CLOSE(-50.0, c.balance(), 0.001);
}

TEST(Pharmacy) {
    Pharmacy p("P1", "Pharm", addrA(), contact(), 1000.0, "INN");
    License valid("L1", pastDate(), futureDate());
    CHECK(p.hasValidLicense(valid));
    License expired("L2", pastDate(), pastDate());
    CHECK(!p.hasValidLicense(expired));
    CHECK_CLOSE(5.0, p.calculateDiscount(), 0.001);
    CHECK(p.canOrderPrescription());
}

TEST(Hospital_DiscountBySize) {
    Hospital big("H1", "Big", addrA(), contact(), 1000.0, "INN", 500);
    CHECK(big.isLarge());
    CHECK_CLOSE(12.0, big.calculateDiscount(), 0.001);
    CHECK(big.needsEmergencySupply());

    Hospital small("H2", "Small", addrA(), contact(), 1000.0, "INN", 100);
    CHECK(!small.isLarge());
    CHECK_CLOSE(7.0, small.calculateDiscount(), 0.001);
}

TEST(Clinic) {
    Clinic c("C1", "Clinic", addrA(), contact(), 500.0, "INN", "cardio");
    CHECK(c.isSpecialized());
    CHECK_CLOSE(3.0, c.calculateDiscount(), 0.001);

    Clinic plain("C2", "Plain", addrA(), contact(), 500.0, "INN", "");
    CHECK(!plain.isSpecialized());
}

// ═══════════════════════ orders.cpp ═══════════════════════

TEST(OrderItem_LineTotalAndDiscount) {
    OrderItem it("M1", "P", 2, 10.0);
    CHECK_CLOSE(20.0, it.lineTotal(), 0.001);
    CHECK(it.isValid());
    it.applyDiscount(50.0);
    CHECK_CLOSE(10.0, it.lineTotal(), 0.001);
    CHECK_THROW(it.applyDiscount(-1.0), InvalidOrderDataException);
    CHECK_THROW(it.applyDiscount(150.0), InvalidOrderDataException);

    CHECK_THROW(OrderItem("M1","P",0,10.0), InvalidOrderDataException);
}

TEST(Payment_ProcessAndRefund) {
    Pharmacy p("C1", "P", addrA(), contact(), 100.0, "INN");
    Payment pay("P1", "I1", "C1", 50.0, std::chrono::system_clock::now(), "card");
    CHECK(!pay.isCompleted());
    CHECK(pay.process(p));
    CHECK(pay.isCompleted());
    CHECK_CLOSE(50.0, pay.amount(), 0.001);
    CHECK(pay.refund());
    CHECK(!pay.isCompleted());

    Pharmacy poor("C2", "P", addrA(), contact(), 10.0, "INN");
    Payment big("P2", "I2", "C2", 1000.0, std::chrono::system_clock::now(), "card");
    CHECK_THROW(big.process(poor), PaymentFailedException);
}

TEST(Invoice_AddPaymentAndRemaining) {
    Invoice inv("I1", "O1", std::chrono::system_clock::now(),
                std::chrono::system_clock::now(), 1000.0);
    CHECK(!inv.isPaid());
    CHECK_CLOSE(1000.0, inv.remaining(), 0.001);
    inv.addPayment(400.0);
    CHECK_CLOSE(600.0, inv.remaining(), 0.001);
    CHECK(!inv.isPaid());
    inv.addPayment(700.0);
    CHECK(inv.isPaid());
}

TEST(Delivery_Lifecycle) {
    Delivery d("D1", "S1", "DR1", "V1", "R1", std::chrono::system_clock::now()
               + std::chrono::hours(1));
    CHECK_THROW(d.assignDriver(""), DriverNotAvailableException);
    CHECK(d.assignDriver("DR2"));
    CHECK(d.start());
    CHECK(d.complete());
    CHECK(d.isOnTime(std::chrono::system_clock::now()));
}

TEST(Shipment_Lifecycle) {
    Shipment s("SH1", "O1", "W1", "WAY1");
    CHECK(!s.isInTransit());
    CHECK(s.markShipped());
    CHECK(s.isInTransit());
    CHECK(s.markDelivered());
    CHECK(!s.isInTransit());
}

TEST(Order_FullFlow) {
    Order o("O1", "C1", std::chrono::system_clock::now());
    o.addItem(OrderItem("M1", "P", 2, 10.0));
    o.addItem(OrderItem("M2", "A", 1, 20.0, 50.0));
    CHECK_CLOSE(30.0, o.calculateTotal(), 0.001);
    CHECK_EQUAL(std::string("draft"), o.status());

    o.submit();
    CHECK_EQUAL(std::string("submitted"), o.status());

    o.removeItem("M1");
    CHECK_EQUAL(size_t(1), o.items().size());
    CHECK_CLOSE(10.0, o.calculateTotal(), 0.001);

    Pharmacy p("C1", "P", addrA(), contact(), 1000.0, "INN");
    Payment pay("P1", "I1", "C1", 10.0, std::chrono::system_clock::now(), "card");
    pay.process(p);
    CHECK(o.confirmPayment(pay));
    CHECK_EQUAL(std::string("paid"), o.status());

    Order o2("O2", "C1", std::chrono::system_clock::now());
    CHECK(o2.cancel());
    CHECK_EQUAL(std::string("cancelled"), o2.status());
}

TEST(Order_EmptySubmitThrows) {
    Order o("O1", "C1", std::chrono::system_clock::now());
    CHECK_THROW(o.submit(), InvalidOrderDataException);
}

TEST(Order_ConfirmPaymentNotCompletedThrows) {
    Order o("O1", "C1", std::chrono::system_clock::now());
    o.addItem(OrderItem("M1", "P", 1, 10.0));
    o.submit();
    Payment pay("P1", "I1", "C1", 10.0, std::chrono::system_clock::now(), "card");
    CHECK_THROW(o.confirmPayment(pay), PaymentFailedException);
}

TEST(Return_RefundAndApproveReject) {
    Return r("R1", "O1", "M1", 3, "damaged", std::chrono::system_clock::now());
    CHECK_CLOSE(30.0, r.calculateRefund(10.0), 0.001);
    CHECK(r.approve());
    CHECK(!r.reject("no"));
}

TEST(DeliverySchedule_OverdueAndReschedule) {
    Date dep = std::chrono::system_clock::now();
    Date arr = dep + std::chrono::hours(2);
    DeliverySchedule ds("V1", "DR1", "O1", dep, arr);
    CHECK(!ds.isOverdue(dep));
    CHECK(ds.isOverdue(arr + std::chrono::hours(1)));
    CHECK(ds.durationMinutes() >= 119);

    ds.reschedule(dep, arr + std::chrono::hours(1));
    CHECK(ds.durationMinutes() >= 179);
}

// ═══════════════════════ warehouse.cpp ═══════════════════════

TEST(Rack_PlaceRemove) {
    Rack r("R1", 10, "Z1");
    CHECK_EQUAL(10, r.freeSpace());
    CHECK(r.placeBatch("B1", 5));
    CHECK_EQUAL(5, r.freeSpace());
    CHECK_THROW(r.placeBatch("B2", 100), WarehouseFullException);
    CHECK(r.removeBatch());
    CHECK_EQUAL(10, r.freeSpace());
}

TEST(StorageZone) {
    StorageZone cold("Z1", "Cold", "W1", 2.0, 8.0, 100);
    CHECK(cold.isTemperatureValid(5.0));
    CHECK(!cold.isTemperatureValid(15.0));
    CHECK(cold.canStore(50));
    CHECK(!cold.canStore(200));
    CHECK(cold.requiresRefrigeration());

    StorageZone warm("Z2", "Warm", "W1", 15.0, 25.0, 100);
    CHECK(!warm.requiresRefrigeration());
}

TEST(Batch) {
    Batch b("B1", "M1", "S1", 100, pastDate(), futureDate(), 5.0);
    CHECK(!b.isExpired());
    CHECK_CLOSE(500.0, b.totalValue(), 0.001);
    CHECK(b.assignRack("R1"));
    CHECK_EQUAL(std::string("B1"), b.id());
    CHECK_EQUAL(std::string("M1"), b.medicineId());
    CHECK_EQUAL(100, b.quantity());
    b.setQuantity(50);
    CHECK_EQUAL(50, b.quantity());
    CHECK_CLOSE(250.0, b.totalValue(), 0.001);

    Batch expired("B2", "M2", "S1", 10, pastDate(), pastDate(), 1.0);
    CHECK(expired.isExpired());
}

TEST(BatchItem_Lifecycle) {
    BatchItem bi("BI1", "B1", "BC1", 10);
    CHECK(!bi.isAvailable());
    CHECK(bi.quarantine());
    CHECK(bi.release());
    CHECK(bi.isAvailable());
}

TEST(Warehouse) {
    Warehouse wh("W1", "Main", addrA(), 100);
    wh.addZone("Z1");
    wh.addZone("Z2");
    CHECK(wh.canAccept(50));
    CHECK(!wh.canAccept(200));
    CHECK_CLOSE(0.0, wh.utilization(), 0.001);
    CHECK(wh.requiresRefrigeration());
    CHECK_EQUAL(std::string("W1"), wh.id());
}

TEST(Inventory) {
    Inventory inv("I1", "W1", "E1", std::chrono::system_clock::now());
    inv.addBatch("B1");
    inv.addBatch("B2");
    CHECK(inv.performCheck());
    CHECK_CLOSE(0.0, inv.discrepancyPercent(), 0.001);
    CHECK(!inv.hasDiscrepancies());
}

// ═══════════════════════ employees.cpp ═══════════════════════

TEST(Employee_Base) {
    Employee e("E1", "Ivanov", addrA(), contact(), "Position",
               pastDate(), 1000.0, "D1");
    CHECK(e.yearsOfService(std::chrono::system_clock::now()) >= 4);
    CHECK(e.calculateBonus() > 0);
    CHECK(e.isEligibleForRaise());
    CHECK_EQUAL(std::string("E1"), e.id());
    CHECK_EQUAL(std::string("Ivanov"), e.fullName());
    CHECK_CLOSE(1000.0, e.salary(), 0.001);
    e.transferToDepartment("D2");
}

TEST(Manager_Bonus) {
    Manager m("E1", "Manager", addrA(), contact(), pastDate(),
              1000.0, "D1", 10);
    CHECK(m.calculateBonus() > 1000.0 * 0.15);
    CHECK(m.approveOrder("O1"));
    m.assignTask("E2");
}

TEST(Pharmacist_DispenseRx) {
    Pharmacist p("E1", "Ivanov", addrA(), contact(), pastDate(),
                 1000.0, "D1", "L1");
    CHECK(p.calculateBonus() > 0);
    CHECK(p.calculateBonus() < p.salary());

    Antibiotic m("M2", "Amoxicillin", 20.0, 50, futureDate(),
                 "MAN", "CAT", "BC2", "form-1", 2.0, "broad", 0.3);
    CHECK(p.dispense(m, "RX1"));
    CHECK_THROW(p.dispense(m, ""), PrescriptionRequiredException);

    Pharmacy c("C1", "P", addrA(), contact(), 100.0, "INN");
    CHECK(p.consult(c));
}

TEST(WarehouseWorker) {
    WarehouseWorker w("E1", "Worker", addrA(), contact(), pastDate(),
                      800.0, "D1");
    Batch b("B1", "M1", "S1", 10, pastDate(), futureDate(), 1.0);
    CHECK(w.receiveBatch(b));
    Batch expired("B2", "M2", "S1", 10, pastDate(), pastDate(), 1.0);
    CHECK(!w.receiveBatch(expired));

    Order o("O1", "C1", std::chrono::system_clock::now());
    o.addItem(OrderItem("M1", "P", 1, 10.0));
    o.submit();
    CHECK(w.shipOrder(o));
    CHECK(w.calculateBonus() > 0);
}

TEST(Driver_AvailabilityAndDrive) {
    Driver d("E1", "Petrov", addrA(), contact(), pastDate(),
             1200.0, "D1", "L1", "V1");
    CHECK(d.isAvailable());
    CHECK(d.canDrive("truck"));
    CHECK(d.canDrive("van"));
    CHECK(d.canDrive("refrigerated"));
    CHECK(!d.canDrive("bike"));
    CHECK(d.calculateBonus() > 0);

    d.setAvailable(false);
    CHECK(!d.isAvailable());
    CHECK_THROW(d.canDrive("truck"), DriverNotAvailableException);
}

TEST(Accountant) {
    Accountant a("E1", "Acc", addrA(), contact(), pastDate(),
                 900.0, "D1");
    Payment p("P1", "I1", "C1", 100.0, std::chrono::system_clock::now(), "card");
    CHECK(!a.processPayment(p));
    Pharmacy c("C1", "P", addrA(), contact(), 100.0, "INN");
    p.process(c);
    CHECK(a.processPayment(p));

    Invoice inv("I1", "O1", std::chrono::system_clock::now(),
                std::chrono::system_clock::now(), 1000.0);
    CHECK(a.issueInvoice(inv));
    CHECK(a.calculateBonus() > 0);
}

TEST(SalesRepresentative) {
    SalesRepresentative s("E1", "Sales", addrA(), contact(), pastDate(),
                          1000.0, "D1", 10000.0);
    Pharmacy c("C1", "P", addrA(), contact(), 100000.0, "INN");
    CHECK(s.makeDeal(c, 5000.0));
    CHECK_CLOSE(50.0, s.progressPercent(), 0.001);
    CHECK_THROW(s.makeDeal(c, -1.0), InvalidOrderDataException);
    CHECK(s.calculateBonus() > 0);
}

TEST(QualityController) {
    QualityController qc("E1", "QC", addrA(), contact(), pastDate(),
                         1100.0, "D1");
    Batch b("B1", "M1", "S1", 10, pastDate(), futureDate(), 1.0);
    CHECK(qc.inspectBatch(b));

    Medicine m("M1", "Med", 10.0, 100, futureDate(),
               "M", "C", 1.0, "tab", false, "BC");
    CHECK(qc.initiateRecall(m));

    Medicine expired("M2", "Med2", 10.0, 100, pastDate(),
                     "M", "C", 1.0, "tab", false, "BC");
    CHECK_THROW(qc.initiateRecall(expired), RecallException);
    CHECK(qc.calculateBonus() > 0);
}

TEST(LogisticsManager) {
    LogisticsManager lm("E1", "LM", addrA(), contact(), pastDate(),
                        1300.0, "D1");
    Route r("R1", {}, 100.0, 60);
    CHECK(lm.optimizeRoute(r));
    CHECK(lm.assignVehicle("O1", "V1"));
    CHECK(lm.calculateBonus() > 0);
}

// ═══════════════════════ transport.cpp ═══════════════════════

TEST(Vehicle_Base) {
    Truck t("V1", "AB123", "Volvo", 5000, 20, 3);
    CHECK(t.isAvailable());
    CHECK(t.assignDriver("DR1"));
    CHECK(t.canTransport("dry"));
    CHECK(!t.canTransport("cold"));
    CHECK_CLOSE(1200.0, t.maxRange(), 0.001);
    CHECK_EQUAL(std::string("V1"), t.id());
    CHECK_CLOSE(5000.0, t.capacityKg(), 0.001);

    t.setAvailable(false);
    CHECK(!t.isAvailable());
    CHECK_THROW(t.assignDriver("DR2"), VehicleNotAvailableException);
}

TEST(Van_MaxRange) {
    Van v("V1", "AB123", "Ford", 1500, 8);
    CHECK_CLOSE(500.0, v.maxRange(), 0.001);
    CHECK(v.canTransport("any"));
}

TEST(RefrigeratedTruck) {
    RefrigeratedTruck t("V1", "AB123", "Volvo", 5000, 20, 3, 2.0, 8.0);
    CHECK(t.checkColdChain(5.0));
    CHECK_THROW(t.checkColdChain(15.0), ColdChainViolationException);
    CHECK_THROW(t.checkColdChain(0.0), ColdChainViolationException);
    CHECK(t.canTransport("cold"));
    CHECK(t.canTransport("vaccine"));
    CHECK(!t.canTransport("dry"));
}

TEST(Route) {
    Route r("R1", {addrA()}, 100.0, 60);
    CHECK_CLOSE(10.0, r.estimateFuel(10.0), 0.001);
    Truck t("V1", "AB123", "Volvo", 5000, 20, 3);
    CHECK(r.isFeasible(t));
    CHECK_EQUAL(std::string("R1"), r.id());

    Route far("R2", {}, 5000.0, 60);
    CHECK(!far.isFeasible(t));

    far.addStop(addrB());
}

// ═══════════════════════ documents.cpp ═══════════════════════

TEST(Document_Base) {
    Document d("D1", "Title", std::chrono::system_clock::now(), "E1");
    CHECK(!d.isSigned());
    CHECK(d.sign("signer"));
    CHECK(d.isSigned());
    CHECK_EQUAL(std::string("Title"), d.render());
    CHECK_EQUAL(std::string("D1"), d.id());
}

TEST(Waybill) {
    Waybill w("W1", std::chrono::system_clock::now(), "E1",
              "S1", "D1", "V1", "R1");
    CHECK(w.validate());
    CHECK(w.matchesShipment("S1"));
    CHECK(!w.matchesShipment("S9"));
    CHECK(w.render().find("W1") != std::string::npos);

    Waybill bad("W2", std::chrono::system_clock::now(), "E1",
                "", "", "", "");
    CHECK(!bad.validate());
}

TEST(Contract) {
    Contract c("C1", "S1", "CUST1", "M1",
               pastDate(), futureDate(), 100000.0);
    CHECK(c.isActive(std::chrono::system_clock::now()));
    CHECK(c.covers("M1"));
    CHECK_CLOSE(100.0, c.calculatePenalty(1), 0.001);
    CHECK(c.renew(12));

    Contract expired("C2", "S1", "CUST1", "M1",
                     pastDate(), pastDate(), 100000.0);
    CHECK(!expired.isActive(std::chrono::system_clock::now()));
}

TEST(PriceList) {
    PriceList pl("PL1", std::chrono::system_clock::now(), "BYN");
    pl.setPrice("M1", 10.0);
    pl.setPrice("M2", 20.0);
    CHECK_CLOSE(10.0, pl.getPrice("M1").value(), 0.001);
    CHECK(!pl.getPrice("X").has_value());
    CHECK_CLOSE(33.0, pl.indexation(10.0), 0.001);
}

TEST(Promotion) {
    Date start = std::chrono::system_clock::now() - std::chrono::hours(1);
    Date end   = std::chrono::system_clock::now() + std::chrono::hours(1);
    Promotion p("PR1", "M1", start, end, 20.0, "sale");
    CHECK(p.isActive(std::chrono::system_clock::now()));
    CHECK_CLOSE(80.0, p.apply(100.0), 0.001);

    Order o("O1", "C1", std::chrono::system_clock::now());
    o.addItem(OrderItem("M1", "P", 1, 10.0));
    CHECK(p.targetsOrder(o));

    Order o2("O2", "C1", std::chrono::system_clock::now());
    o2.addItem(OrderItem("M9", "X", 1, 10.0));
    CHECK(!p.targetsOrder(o2));

    Date old = std::chrono::system_clock::now() - std::chrono::hours(100);
    Promotion expired("PR2", "M1", old, old, 10.0, "");
    CHECK(!expired.isActive(std::chrono::system_clock::now()));
}

TEST(Discount) {
    Discount d("D1", "C1", "O1", 10.0, "loyal");
    CHECK_CLOSE(90.0, d.apply(100.0), 0.001);
    CHECK(d.isStackable());

    Discount big("D2", "C1", "O1", 25.0, "sale");
    CHECK(!big.isStackable());
}

TEST(PromotionCode) {
    Date future = std::chrono::system_clock::now() + std::chrono::hours(24);
    PromotionCode pc("CODE", "D1", future, 2);
    CHECK(pc.isValid());
    CHECK_EQUAL(2, pc.remaining());
    CHECK(pc.redeem());
    CHECK_EQUAL(1, pc.remaining());
    CHECK(pc.redeem());
    CHECK(!pc.redeem());
    CHECK(!pc.isValid());

    Date past = std::chrono::system_clock::now() - std::chrono::hours(24);
    PromotionCode expired("CODE2", "D1", past, 5);
    CHECK(!expired.isValid());
    CHECK(!expired.redeem());
}

// ═══════════════════════ reports.cpp ═══════════════════════

TEST(SalesReport) {
    SalesReport sr("R1", std::chrono::system_clock::now(), "E1",
                   "O1", "M1", 1000.0);
    CHECK_CLOSE(1000.0, sr.summarize(), 0.001);
    CHECK_CLOSE(100.0, sr.averageOrderValue(10), 0.001);
    CHECK_CLOSE(0.0, sr.averageOrderValue(0), 0.001);
    CHECK(sr.render().find("1000") != std::string::npos);
    CHECK_EQUAL(std::string("R1"), sr.id());
}

TEST(StockReport) {
    StockReport sr("R1", std::chrono::system_clock::now(), "E1",
                   "W1", "B1", 5);
    CHECK_CLOSE(5.0, sr.summarize(), 0.001);
    CHECK(sr.isLowStock(10));
    CHECK(!sr.isLowStock(3));
    CHECK(sr.render().find("5") != std::string::npos);
}

TEST(FinancialReport) {
    FinancialReport fr("R1", std::chrono::system_clock::now(), "E1",
                       "P1", "I1", 1000.0);
    CHECK_CLOSE(1000.0, fr.summarize(), 0.001);
    CHECK_CLOSE(200.0, fr.tax(20.0), 0.001);
    CHECK(fr.render().find("1000") != std::string::npos);
}

TEST(Report_ExportTo) {
    SalesReport sr("R1", std::chrono::system_clock::now(), "E1",
                   "O1", "M1", 100.0);
    CHECK(sr.exportTo("test_report.txt"));
    CHECK(!sr.exportTo("/nonexistent_dir_xyz/file.txt"));
}

// ═══════════════════════ security.cpp ═══════════════════════

TEST(Permission) {
    Permission p("READ", "Read access", "documents");
    CHECK(p.matches("READ"));
    CHECK(!p.matches("WRITE"));
    CHECK_EQUAL(std::string("READ"), p.code());
}

TEST(Role) {
    Role r("R1", "Admin");
    r.addPermission("read");
    r.addPermission("write");
    CHECK(r.hasPermission("read"));
    CHECK(r.hasPermission("write"));
    CHECK(!r.hasPermission("delete"));
    CHECK_EQUAL(size_t(2), r.permissionCount());
}

TEST(User_AuthenticateChangeLink) {
    User u("U1", "admin", "secret", "R1");
    CHECK(u.authenticate("secret"));
    CHECK(!u.authenticate("wrong"));
    CHECK(!u.changePassword("bad", "newpwd"));
    CHECK(u.changePassword("secret", "newpwd"));
    CHECK(u.authenticate("newpwd"));

    u.linkToEmployee("E1");
    u.linkToCustomer("C1");
}

TEST(AuditLog) {
    AuditLog log("L1", "U1", std::chrono::system_clock::now(),
                 "delete", "removed user");
    CHECK(log.isCritical());
    CHECK(log.filterByUser("U1"));
    CHECK(!log.filterByUser("U2"));
    CHECK(log.format().find("delete") != std::string::npos);

    AuditLog info("L2", "U1", std::chrono::system_clock::now(),
                  "login", "user logged in");
    CHECK(!info.isCritical());
}

TEST(Notification) {
    Notification n("N1", "urgent message", std::chrono::system_clock::now());
    CHECK(n.isImportant());
    n.sendToUser("U1");
    n.sendToCustomer("C1");

    Notification n2("N2", "hello", std::chrono::system_clock::now());
    CHECK(!n2.isImportant());
}

// ═══════════════════════ quality.cpp ═══════════════════════

TEST(Recall_FullLifecycle) {
    Recall r("RC1", "M1", "B1", "contamination",
             std::chrono::system_clock::now());
    CHECK(r.isActive());
    CHECK(r.notifyCustomer("C1"));
    CHECK(r.close());
    CHECK(!r.isActive());
}

TEST(AdverseEvent) {
    AdverseEvent ae("AE1", "M1", "C1", "rash", "critical",
                    std::chrono::system_clock::now());
    CHECK(ae.isSerious());
    CHECK(ae.reportToAuthority("MOH"));
    ae.attachEmployee("E1");

    AdverseEvent mild("AE2", "M1", "C1", "mild rash", "low",
                      std::chrono::system_clock::now());
    CHECK(!mild.isSerious());
}

TEST(AdverseEvent_EmptySeverityThrows) {
    AdverseEvent ae("AE1", "M1", "C1", "rash", "",
                    std::chrono::system_clock::now());
    CHECK_THROW(ae.reportToAuthority("MOH"), AdverseEventException);
}

TEST(ClinicalTrial) {
    ClinicalTrial ct("CT1", "M1", std::chrono::system_clock::now());
    CHECK(ct.addParticipant("C1"));
    CHECK(!ct.addParticipant(""));
    CHECK_CLOSE(80.0, ct.successRate(8, 10), 0.001);
    CHECK_CLOSE(0.0, ct.successRate(0, 0), 0.001);
    CHECK(ct.conclude("positive"));
}

// ═══════════════════════ main ═══════════════════════

int main() {
    return UnitTest::RunAllTests();
}