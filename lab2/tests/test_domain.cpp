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

using namespace pharma;

// ─────────── Address ───────────
TEST(Address_ValidateAndFormat) {
    Address a("A", "Minsk", "220000", "BY", 53.9, 27.5);
    CHECK(a.validate());
    CHECK(a.formatFull().find("Minsk") != std::string::npos);
    CHECK(a.isInCountry("BY"));
}

TEST(Address_Distance) {
    Address a("A", "Minsk", "220000", "BY", 53.9, 27.5);
    Address b("B", "Minsk", "220000", "BY", 53.91, 27.51);
    CHECK(a.distanceTo(b) > 0.0);
}

// ─────────── ContactInfo ───────────
TEST(Contact_ValidateAndMask) {
    ContactInfo c("+375291234567", "user@mail.com");
    CHECK(c.validate());
    CHECK_EQUAL("4567", c.maskPhone().substr(c.maskPhone().size() - 4));
}

TEST(Contact_InvalidEmail) {
    ContactInfo c("+375", "bad-email");
    CHECK(!c.validate());
}

// ─────────── Medicine ───────────
TEST(Medicine_AnalgesicProperties) {
    Analgesic m("M1", "Paracetamol", 5.0, 100,
                makeDate(2030,1,1), "MAN", "CAT", "BC", 5, 12,
                "headache", "mild");
    CHECK(!m.isExpired());
    CHECK(!m.needsPrescription());
    CHECK(m.recommendForPain("headache"));
    CHECK_CLOSE(4.5, m.calculateDiscount(10), 0.001);
}

TEST(Medicine_AntibioticPrescription) {
    Antibiotic ab("M2", "Amoxi", 20.0, 50, makeDate(2030,1,1), "MAN", "CAT",
                  "BC2", "f", 2.0, "broad", 0.3);
    CHECK(ab.needsPrescription());
    CHECK(ab.isBroadSpectrum());
}

TEST(Medicine_NegativePriceThrows) {
    CHECK_THROW(
        Medicine("X","Y",-1.0,1,makeDate(2030,1,1),"M","C",1.0,"t",false,"b"),
        InvalidMedicineDataException);
}

// ─────────── Cold chain ───────────
TEST(Vaccine_RequiresColdChain) {
    Vaccine v("V1", "V", 10, 100, makeDate(2030,1,1), "MAN", "CAT", "BC",
              "f", 0.5, 4.0, 5, "mRNA");
    CHECK(v.requiresColdChain());
    CHECK_THROW(v.checkTemperature(20.0), ColdChainViolationException);
    CHECK(v.checkTemperature(5.0));
}

// ─────────── Order ───────────
TEST(Order_AddRemoveItems) {
    Order o("O1", "C1", std::chrono::system_clock::now());
    o.addItem(OrderItem("M1", "P", 2, 10.0));
    o.addItem(OrderItem("M2", "A", 1, 20.0, 50.0));
    CHECK_CLOSE(30.0, o.calculateTotal(), 0.001);
    o.submit();
    CHECK_EQUAL(std::string("submitted"), o.status());
    o.removeItem("M1");
    CHECK_EQUAL(size_t(1), o.items().size());
}

TEST(Order_EmptySubmitThrows) {
    Order o("O1", "C1", std::chrono::system_clock::now());
    CHECK_THROW(o.submit(), InvalidOrderDataException);
}

// ─────────── Payment ───────────
TEST(Payment_SuccessAndInsufficientFunds) {
    Pharmacy p("C1", "P", Address("A","M","220000","BY",0,0),
               ContactInfo("+375","a@b.c"), 100.0, "INN");
    Payment pay("P1", "I1", "C1", 50.0,
                std::chrono::system_clock::now(), "card");
    CHECK(pay.process(p));
    CHECK_CLOSE(50.0, p.balance(), 0.001);

    Payment big("P2", "I2", "C1", 1000.0,
                std::chrono::system_clock::now(), "card");
    CHECK_THROW(big.process(p), PaymentFailedException);
}

// ─────────── Warehouse ───────────
TEST(Warehouse_CapacityAndBatchValue) {
    Warehouse wh("W1", "Main",
                 Address("A","M","220000","BY",0,0), 100);
    CHECK(wh.canAccept(50));
    CHECK(!wh.canAccept(200));

    Batch b("B1","M1","S1",100, makeDate(2024,1,1),
            makeDate(2030,1,1), 5.0);
    CHECK_CLOSE(500.0, b.totalValue(), 0.001);
    CHECK(!b.isExpired());
}

TEST(Rack_OverflowThrows) {
    Rack r("R1", 10, "Z1");
    CHECK_THROW(r.placeBatch("B1", 100), WarehouseFullException);
}

// ─────────── Employees ───────────
TEST(Pharmacist_DispenseRequiresPrescription) {
    Pharmacist p("E1","Ivanov",
                 Address("A","M","220000","BY",0,0),
                 ContactInfo("+375","a@b.c"),
                 makeDate(2020,1,1), 1000, "D1", "L1");
    CHECK(p.calculateBonus() > 0);
    CHECK(p.calculateBonus() < p.salary());

    Antibiotic m("M2","Amoxicillin",20.0,50, makeDate(2030,1,1),
                 "MAN","CAT","BC2","form-1",2.0,"broad",0.3);
    CHECK(p.dispense(m, "RX1"));
    CHECK_THROW(p.dispense(m, ""), PrescriptionRequiredException);
}

TEST(Driver_AvailabilityAndVehicleType) {
    Driver d("E2","Petrov",
             Address("A","M","220000","BY",0,0),
             ContactInfo("+375","d@b.c"),
             makeDate(2021,1,1), 1200, "D2", "L2", "V1");
    CHECK(d.isAvailable());
    CHECK(d.canDrive("truck"));
    d.setAvailable(false);
    CHECK_THROW(d.canDrive("truck"), DriverNotAvailableException);
}

// ─────────── Transport ───────────
TEST(RefrigeratedTruck_ColdChain) {
    RefrigeratedTruck t("V1","AB123","Volvo",5000,20,3, 2.0, 8.0);
    CHECK(t.checkColdChain(5.0));
    CHECK_THROW(t.checkColdChain(15.0), ColdChainViolationException);
}

TEST(Route_FuelAndFeasibility) {
    RefrigeratedTruck t("V1","AB123","Volvo",5000,20,3, 2.0, 8.0);
    Route r("R1", {}, 100.0, 60);
    CHECK_CLOSE(10.0, r.estimateFuel(10.0), 0.001);
    CHECK(r.isFeasible(t));
}

// ─────────── Documents ───────────
TEST(Waybill_ValidateAndMatch) {
    Waybill w("W1", std::chrono::system_clock::now(),
              "E1", "S1", "D1", "V1", "R1");
    CHECK(w.validate());
    CHECK(w.matchesShipment("S1"));
}

TEST(PriceList_SetGetPrice) {
    PriceList pl("PL1", std::chrono::system_clock::now(), "BYN");
    pl.setPrice("M1", 10.0);
    CHECK_CLOSE(10.0, pl.getPrice("M1").value(), 0.001);
    CHECK(!pl.getPrice("X").has_value());
}

TEST(PromotionCode_Redeem) {
    PromotionCode pc("CODE", "D1", std::chrono::system_clock::now()
                     + std::chrono::hours(24), 2);
    CHECK(pc.redeem());
    CHECK(pc.redeem());
    CHECK(!pc.redeem());
}

// ─────────── Reports ───────────
TEST(SalesReport_SummaryAndAverage) {
    SalesReport sr("R1", std::chrono::system_clock::now(), "E1",
                   "O1", "M1", 1000.0);
    CHECK_CLOSE(1000.0, sr.summarize(), 0.001);
    CHECK_CLOSE(100.0, sr.averageOrderValue(10), 0.001);
}

TEST(FinancialReport_Tax) {
    FinancialReport fr("R2", std::chrono::system_clock::now(), "E1",
                       "P1", "I1", 1000.0);
    CHECK_CLOSE(200.0, fr.tax(20.0), 0.001);
}

TEST(StockReport_LowStock) {
    StockReport sr("R3", std::chrono::system_clock::now(), "E1",
                   "W1", "B1", 5);
    CHECK(sr.isLowStock(10));
    CHECK(!sr.isLowStock(3));
}

// ─────────── Security ───────────
TEST(User_AuthenticateAndChangePassword) {
    User u("U1","admin","secret","R1");
    CHECK(u.authenticate("secret"));
    CHECK(!u.authenticate("wrong"));
    CHECK(u.changePassword("secret", "newpwd"));
    CHECK(u.authenticate("newpwd"));
    CHECK(!u.changePassword("bad", "x"));
}

TEST(Role_Permissions) {
    Role r("R1","Admin");
    r.addPermission("read");
    CHECK(r.hasPermission("read"));
    CHECK(!r.hasPermission("write"));
    CHECK_EQUAL(size_t(1), r.permissionCount());
}

TEST(AuditLog_Critical) {
    AuditLog log("L1","U1", std::chrono::system_clock::now(),
                 "delete","removed user");
    CHECK(log.isCritical());
    CHECK(log.filterByUser("U1"));
}

// ─────────── Quality ───────────
TEST(Recall_Lifecycle) {
    Recall r("RC1","M1","B1","contamination",
             std::chrono::system_clock::now());
    CHECK(r.isActive());
    CHECK(r.notifyCustomer("C1"));
    CHECK(r.close());
    CHECK(!r.isActive());
}

TEST(AdverseEvent_SeriousAndReport) {
    AdverseEvent ae("AE1","M1","C1","rash","critical",
                    std::chrono::system_clock::now());
    CHECK(ae.isSerious());
    CHECK(ae.reportToAuthority("MOH"));
}

TEST(ClinicalTrial_SuccessRate) {
    ClinicalTrial ct("CT1","M1", std::chrono::system_clock::now());
    CHECK(ct.addParticipant("C1"));
    CHECK_CLOSE(80.0, ct.successRate(8, 10), 0.001);
    CHECK(ct.conclude("positive"));
}

// ─────────── run ───────────
int main() {
    return UnitTest::RunAllTests();
}