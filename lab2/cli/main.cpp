#include "core.hpp"
#include "orders.hpp"
#include "warehouse.hpp"
#include "employees.hpp"
#include "transport.hpp"
#include "documents.hpp"
#include "reports.hpp"
#include "security.hpp"
#include "quality.hpp"

#include <iostream>
#include <memory>
#include <vector>

using namespace pharma;

namespace {

void printHeader() {
    std::cout << "=== Pharma Distribution CLI ===\n"
              << "1. Каталог препаратов\n"
              << "2. Оформить заказ\n"
              << "3. Склад и партии\n"
              << "4. Сотрудники\n"
              << "5. Транспорт и доставка\n"
              << "6. Отчёты\n"
              << "7. Безопасность\n"
              << "8. Качество\n"
              << "0. Выход\n";
}

void listMedicines(const std::vector<std::unique_ptr<Medicine>>& catalog) {
    for (const auto& m : catalog)
        std::cout << " - " << m->getFullInfo() << "\n";
}

void demoOrder(Customer& c) {
    Order o("ORD-1", c.id(), std::chrono::system_clock::now());
    o.addItem(OrderItem("MED-1", "Paracetamol", 10, 5.0));
    o.submit();
    std::cout << "Order total: " << o.calculateTotal() << "\n";
    Payment p("PAY-1", "INV-1", c.id(), o.calculateTotal(),
              std::chrono::system_clock::now(), "card");
    if (p.process(c)) std::cout << "Payment OK\n";
}

void demoWarehouse() {
    Warehouse wh("WH-1", "Main",
                 Address("Lenina 1", "Minsk", "220000", "BY", 53.9, 27.5),
                 10000);
    Batch b("B-1", "MED-1", "SUP-1", 500,
            makeDate(2024,1,1), makeDate(2027,1,1), 3.5);
    std::cout << "Batch value: " << b.totalValue() << "\n";
    std::cout << "WH util: " << wh.utilization() << "%\n";
}

void demoEmployee() {
    Pharmacist ph("E-1", "Ivanov I.I.",
                  Address("A", "Minsk", "220000", "BY", 53.9, 27.5),
                  ContactInfo("+375...", "ph@pharma.by"), makeDate(2020,1,1),
                  1500, "D-1", "LIC-1");
    std::cout << "Bonus: " << ph.calculateBonus() << "\n";
}

} // namespace

int main() {
    Address addr("Nezavisimosti 5", "Minsk", "220000", "BY", 53.9, 27.5);
    ContactInfo ci("+375291234567", "pharmacy@mail.by");
    Pharmacy pharmacy("CUST-1", "Apteka №1", addr, ci, 100000, "1234567890");

    std::vector<std::unique_ptr<Medicine>> catalog;
    catalog.push_back(std::make_unique<Analgesic>(
        "MED-1", "Paracetamol", 5.0, 1000,
        makeDate(2026,1,1), "MAN-1", "CAT-1", "BC-001",
        5, 12, "headache", "mild"));
    catalog.push_back(std::make_unique<Antibiotic>(
        "MED-2", "Amoxicillin", 20.0, 300,
        makeDate(2026,6,1), "MAN-2", "CAT-2", "BC-002",
        "form-1", 2.0, "broad", 0.3));
    catalog.push_back(std::make_unique<Vaccine>(
        "MED-3", "COVID-Vac", 60.0, 200,
        makeDate(2026,9,1), "MAN-3", "CAT-3", "BC-003",
        "form-2", 0.5, 4.0, 5, "mRNA"));

    while (true) {
        printHeader();
        std::cout << "> ";
        int choice = -1;
        if (!(std::cin >> choice)) break;
        if (choice == 0) break;
        try {
            switch (choice) {
                case 1: listMedicines(catalog); break;
                case 2: demoOrder(pharmacy); break;
                case 3: demoWarehouse(); break;
                case 4: demoEmployee(); break;
                case 5: {
                    Route r("R-1", {addr}, 120.0, 90);
                    RefrigeratedTruck t("V-1", "AB1234", "Volvo",
                                        5000, 20, 3, 2.0, 8.0);
                    std::cout << "Feasible: " << r.isFeasible(t) << "\n";
                    break;
                }
                case 6: {
                    SalesReport sr("REP-1", std::chrono::system_clock::now(),
                                   "E-1", "ORD-1", "MED-1", 500.0);
                    std::cout << sr.render()
                              << " sum=" << sr.summarize() << "\n";
                    break;
                }
                case 7: {
                    User u("U-1", "admin", "secret", "ROLE-1");
                    std::cout << "Auth: " << u.authenticate("secret") << "\n";
                    break;
                }
                case 8: {
                    QualityController qc("E-2", "Petrov",
                        Address("B", "Minsk", "220000", "BY", 53.9, 27.5),
                        ContactInfo("+375...", "q@pharma.by"),
                        makeDate(2021,1,1), 1800, "D-2");
                    std::cout << "Inspect: " << qc.inspectBatch(
                        Batch("B-1","MED-1","SUP-1",10,
                              makeDate(2024,1,1), makeDate(2027,1,1), 3.0))
                              << "\n";
                    break;
                }
                default: std::cout << "Unknown option\n";
            }
        } catch (const PharmaException& e) {
            std::cerr << "[domain error] " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "[error] " << e.what() << "\n";
        }
    }
    return 0;
}