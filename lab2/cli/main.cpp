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

#ifdef _WIN32
#include <windows.h>
#endif

using namespace pharma;

//  вспомогательные функции для меню 

void printMenu() {
    std::cout << "\n=== Меню ===\n";
    std::cout << "1 - Каталог препаратов\n";
    std::cout << "2 - Оформить заказ\n";
    std::cout << "3 - Работа со складом\n";
    std::cout << "4 - Сотрудники и бонусы\n";
    std::cout << "5 - Транспорт и маршрут\n";
    std::cout << "6 - Отчёты\n";
    std::cout << "7 - Безопасность\n";
    std::cout << "8 - Контроль качества\n";
    std::cout << "0 - Выход\n";
    std::cout << "Ваш выбор: ";
}

// 1. Каталог 

void showCatalog(const std::vector<std::unique_ptr<Medicine>>& catalog) {
    std::cout << "\n--- Каталог препаратов ---\n";
    for (const auto& m : catalog) {
        std::cout << m->id() << " | " << m->name()
                  << " | цена: " << m->price()
                  << " | остаток: " << m->quantity()
                  << " | рецепт: " << (m->needsPrescription() ? "нужен" : "не нужен")
                  << "\n";
    }
    std::cout << "Всего препаратов: " << catalog.size() << "\n";
}

//  2. Заказ

void makeOrder(Customer& customer) {
    std::cout << "\n--- Оформление заказа ---\n";
    std::cout << "Клиент: " << customer.name() << "\n";
    std::cout << "Баланс до заказа: " << customer.balance() << "\n";

    Order order("ORD-1", customer.id(), std::chrono::system_clock::now());
    order.addItem(OrderItem("MED-1", "Paracetamol", 10, 5.0));
    order.addItem(OrderItem("MED-2", "Amoxicillin", 2, 20.0, 10.0));

    std::cout << "Позиции добавлены.\n";
    std::cout << "Сумма заказа: " << order.calculateTotal() << "\n";

    order.submit();
    std::cout << "Статус заказа: " << order.status() << "\n";

    // пробуем оплатить
    Payment payment("PAY-1", "INV-1", customer.id(),
                    order.calculateTotal(),
                    std::chrono::system_clock::now(), "card");

    if (payment.process(customer)) {
        std::cout << "Оплата прошла успешно.\n";
        std::cout << "Баланс после оплаты: " << customer.balance() << "\n";
        order.confirmPayment(payment);
        std::cout << "Статус заказа после оплаты: " << order.status() << "\n";
    }
}

//3. Склад 

void workWithWarehouse() {
    std::cout << "\n--- Склад ---\n";

    Warehouse wh("WH-1", "Главный склад",
                 Address("Ленина 1", "Минск", "220000", "BY", 53.9, 27.5),
                 10000);
    std::cout << "Создан склад: " << wh.id() << " вместимость 10000\n";

    Batch batch("B-1", "MED-1", "SUP-1", 500,
                makeDate(2024, 1, 1), makeDate(2027, 1, 1), 3.5);
    std::cout << "Создана партия на 500 упаковок\n";
    std::cout << "Срок годности до 2027 года. Просрочена? "
              << (batch.isExpired() ? "да" : "нет") << "\n";
    std::cout << "Стоимость партии: " << batch.totalValue() << "\n";
    std::cout << "Заполнение склада: " << wh.utilization() << "%\n";
}

//  4. Сотрудники 

void showEmployees() {
    std::cout << "\n--- Сотрудники ---\n";

    Address addr("Независимости 5", "Минск", "220000", "BY", 53.9, 27.5);
    ContactInfo ci("+375291234567", "emp@pharma.by");

    Pharmacist ph("E-1", "Иванов И.И.", addr, ci,
                  makeDate(2020, 1, 1), 1500, "D-1", "LIC-1");
    Manager mgr("E-2", "Петров П.П.", addr, ci,
                makeDate(2018, 1, 1), 2500, "D-1", 10);
    Driver drv("E-3", "Сидоров С.С.", addr, ci,
               makeDate(2022, 1, 1), 1200, "D-2", "DRV-1", "V-1");

    std::cout << "Фармацевт " << ph.fullName()
              << ": оклад " << ph.salary()
              << ", бонус " << ph.calculateBonus() << "\n";

    std::cout << "Менеджер " << mgr.fullName()
              << ": оклад " << mgr.salary()
              << ", бонус " << mgr.calculateBonus() << "\n";

    std::cout << "Водитель " << drv.fullName()
              << ": оклад " << drv.salary()
              << ", бонус " << drv.calculateBonus() << "\n";

    std::cout << "У каждого своя формула бонуса (полиморфизм).\n";
}

//  5. Транспорт 

void showTransport(const Address& addr) {
    std::cout << "\n--- Транспорт ---\n";

    Route route("R-1", {addr}, 120.0, 90);
    std::cout << "Создан маршрут на 120 км\n";

    RefrigeratedTruck truck("V-1", "AB1234", "Volvo",
                            5000, 20, 3, 2.0, 8.0);
    std::cout << "Создан рефрижератор, дальность до 1200 км\n";

    std::cout << "Хватит ли машине маршрута? "
              << (route.isFeasible(truck) ? "да" : "нет") << "\n";

    // проверяем холод
    std::cout << "Проверяем температуру 5 градусов:\n";
    truck.checkColdChain(5.0);
    std::cout << "  всё нормально\n";

    std::cout << "Проверяем температуру 15 градусов:\n";
    try {
        truck.checkColdChain(15.0);
    } catch (const ColdChainViolationException& e) {
        std::cout << "  ошибка: " << e.what() << "\n";
    }
}

// 6. Отчёты 

void showReports() {
    std::cout << "\n--- Отчёты ---\n";

    SalesReport sales("R-1", std::chrono::system_clock::now(),
                      "E-1", "ORD-1", "MED-1", 500.0);
    std::cout << sales.render() << "\n";
    std::cout << "Итог: " << sales.summarize() << "\n";
    std::cout << "Средний чек (10 заказов): "
              << sales.averageOrderValue(10) << "\n";

    StockReport stock("R-2", std::chrono::system_clock::now(),
                      "E-1", "WH-1", "B-1", 150);
    std::cout << stock.render() << "\n";
    std::cout << "Итог: " << stock.summarize() << "\n";
    std::cout << "Мало товара (< 200)? "
              << (stock.isLowStock(200) ? "да" : "нет") << "\n";

    FinancialReport fin("R-3", std::chrono::system_clock::now(),
                        "E-1", "PAY-1", "INV-1", 1000.0);
    std::cout << fin.render() << "\n";
    std::cout << "Итог: " << fin.summarize() << "\n";
    std::cout << "Налог 20%: " << fin.tax(20.0) << "\n";
}

// 7. Безопасность 
void showSecurity() {
    std::cout << "\n--- Безопасность ---\n";

    Role admin("ROLE-1", "Admin");
    admin.addPermission("READ");
    admin.addPermission("WRITE");
    admin.addPermission("DELETE");
    std::cout << "Роль Admin, прав: " << admin.permissionCount() << "\n";

    User user("U-1", "admin", "secret", "ROLE-1");

    std::cout << "Проверка пароля 'secret': "
              << (user.authenticate("secret") ? "успех" : "провал") << "\n";
    std::cout << "Проверка пароля 'wrong': "
              << (user.authenticate("wrong") ? "успех" : "провал") << "\n";

    std::cout << "Меняем пароль со старого 'wrong': "
              << (user.changePassword("wrong", "newpass") ? "успех" : "отказ") << "\n";
    std::cout << "Меняем пароль со старого 'secret': "
              << (user.changePassword("secret", "newpass") ? "успех" : "отказ") << "\n";
    std::cout << "Проверка нового пароля: "
              << (user.authenticate("newpass") ? "успех" : "провал") << "\n";
}

// 8. Качество 

void showQuality() {
    std::cout << "\n--- Контроль качества ---\n";

    QualityController qc("E-2", "Петров П.П.",
        Address("Независимости 5", "Минск", "220000", "BY", 53.9, 27.5),
        ContactInfo("+375291234567", "qc@pharma.by"),
        makeDate(2021, 1, 1), 1800, "D-2");

    std::cout << "Контролёр: " << qc.fullName()
              << ", бонус " << qc.calculateBonus() << "\n";

    Batch good("B-1", "MED-1", "SUP-1", 100,
               makeDate(2024, 1, 1), makeDate(2027, 1, 1), 3.0);
    std::cout << "Проверка партии до 2027: "
              << (qc.inspectBatch(good) ? "OK" : "брак") << "\n";

    Medicine med("M-1", "Paracetamol", 5.0, 100,
                 makeDate(2030, 1, 1), "MAN", "CAT", 1.0, "tab", false, "BC");
    std::cout << "Отзыв годного лекарства: "
              << (qc.initiateRecall(med) ? "одобрен" : "отказ") << "\n";

    std::cout << "Отзыв просроченного лекарства:\n";
    try {
        Medicine expired("M-2", "Old", 5.0, 100,
                         makeDate(2020, 1, 1), "MAN", "CAT", 1.0, "tab", false, "BC");
        qc.initiateRecall(expired);
    } catch (const RecallException& e) {
        std::cout << "  ошибка: " << e.what() << "\n";
    }
}

//  main 

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    Address addr("Независимости 5", "Минск", "220000", "BY", 53.9, 27.5);
    ContactInfo ci("+375291234567", "pharmacy@mail.by");
    Pharmacy pharmacy("CUST-1", "Аптека №1", addr, ci, 100000, "1234567890");

    // каталог лекарств
    std::vector<std::unique_ptr<Medicine>> catalog;
    catalog.push_back(std::make_unique<Analgesic>(
        "MED-1", "Paracetamol", 5.0, 1000,
        makeDate(2026, 1, 1), "MAN-1", "CAT-1", "BC-001",
        5, 12, "headache", "mild"));
    catalog.push_back(std::make_unique<Antibiotic>(
        "MED-2", "Amoxicillin", 20.0, 300,
        makeDate(2026, 6, 1), "MAN-2", "CAT-2", "BC-002",
        "form-1", 2.0, "broad", 0.3));
    catalog.push_back(std::make_unique<Vaccine>(
        "MED-3", "COVID-Vac", 60.0, 200,
        makeDate(2026, 9, 1), "MAN-3", "CAT-3", "BC-003",
        "form-2", 0.5, 4.0, 5, "mRNA"));

    std::cout << "Программа готова к работе.\n";

    int choice = -1;
    while (true) {
        printMenu();
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Выход из программы.\n";
            break;
        }

        try {
            if (choice == 1) {
                showCatalog(catalog);
            } else if (choice == 2) {
                makeOrder(pharmacy);
            } else if (choice == 3) {
                workWithWarehouse();
            } else if (choice == 4) {
                showEmployees();
            } else if (choice == 5) {
                showTransport(addr);
            } else if (choice == 6) {
                showReports();
            } else if (choice == 7) {
                showSecurity();
            } else if (choice == 8) {
                showQuality();
            } else {
                std::cout << "Неверный пункт меню.\n";
            }
        } catch (const PharmaException& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }

    return 0;
}