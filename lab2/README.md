# Lab2 — Компания по сбыту лекарственных препаратов

Крупный ООП-проект на C++17. Предметная область — фармацевтическая
дистрибьюторская компания: закупка, хранение, продажа и доставка
лекарственных препаратов.

## Соответствие требованиям

| Показатель              | Требуется | Реализовано |
|-------------------------|-----------|-------------|
| Классы                  | ≥ 50      | **68**      |
| Поля                    | ≥ 150     | **384**     |
| Поведения (методы)      | ≥ 100     | **206**     |
| Ассоциации              | ≥ 30      | **74**      |
| Собственные исключения  | ≥ 12      | **20**      |

## Сборка

```bash
cmake -S lab2 -B lab2/build -DCMAKE_BUILD_TYPE=Release
cmake --build lab2/build
```

## Запуск CLI

```bash
./lab2/build/pharma_app
```

## Тесты

```bash
ctest --test-dir lab2/build --output-on-failure
```

## Покрытие

```bash
cmake -S lab2 -B lab2/build-cov -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build lab2/build-cov
ctest --test-dir lab2/build-cov
cd lab2 && gcovr --root . --filter "src/.*" --print-summary --fail-under-line 90
```

## Структура

```
lab2/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/
│   ├── exceptions.hpp
│   ├── core.hpp
│   ├── orders.hpp
│   ├── warehouse.hpp
│   ├── employees.hpp
│   ├── transport.hpp
│   ├── documents.hpp
│   ├── reports.hpp
│   ├── security.hpp
│   └── quality.hpp
├── src/
│   ├── core.cpp
│   ├── orders.cpp
│   ├── warehouse.cpp
│   ├── employees.cpp
│   ├── transport.cpp
│   ├── documents.cpp
│   ├── reports.cpp
│   ├── security.cpp
│   └── quality.cpp
├── cli/main.cpp
└── tests/test_domain.cpp
```

## Классы, поля, методы, ассоциации

### Ядро домена

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| Address | 6 | 5 | — |
| ContactInfo | 4 | 3 | — |
| Category | 4 | 2 | Medicine |
| Medicine | 12 | 5 | Manufacturer, Category, Batch, OrderItem |
| PrescriptionMedicine | 2 | 2 | Medicine |
| OverTheCounterMedicine | 2 | 2 | Medicine |
| Vaccine | 3 | 2 | PrescriptionMedicine |
| Antibiotic | 2 | 2 | PrescriptionMedicine |
| Analgesic | 2 | 2 | OverTheCounterMedicine |
| Manufacturer | 6 | 3 | Medicine, Contract, License |
| Supplier | 6 | 3 | Medicine, Contract, SupplierRating |
| SupplierRating | 4 | 2 | Supplier |
| License | 3 | 2 | Manufacturer, Pharmacy |
| Certificate | 3 | 2 | Medicine, Batch |
| Customer | 7 | 4 | Address, ContactInfo, Order, Payment |
| Pharmacy | 3 | 3 | Address, Order, Customer, License |
| Hospital | 4 | 3 | Address, Order, Customer |
| Clinic | 2 | 2 | Address, Customer |

### Заказы и финансы

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| Order | 8 | 6 | Customer, OrderItem, Payment, Delivery, Address |
| OrderItem | 5 | 3 | Medicine, Order |
| Invoice | 7 | 3 | Order, Payment |
| Payment | 7 | 3 | Invoice, Customer |
| Shipment | 6 | 3 | Order, Warehouse, Waybill |
| Delivery | 7 | 3 | Shipment, Driver, Vehicle, Route |
| Return | 7 | 3 | Order, Medicine, Payment |
| DeliverySchedule | 6 | 3 | Vehicle, Driver, Order |

### Склад

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| Warehouse | 7 | 4 | Address, StorageZone, Employee, Batch |
| StorageZone | 6 | 3 | Rack, Warehouse |
| Rack | 5 | 3 | StorageZone, Batch |
| Batch | 8 | 3 | Medicine, Supplier, Rack |
| BatchItem | 5 | 3 | Batch |
| Inventory | 6 | 4 | Warehouse, Employee, Batch |

### Сотрудники

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| Employee | 8 | 4 | Address, ContactInfo, Department |
| Manager | 2 | 3 | Order, Employee |
| Pharmacist | 2 | 3 | PrescriptionMedicine, Customer |
| WarehouseWorker | 2 | 3 | Order, Warehouse |
| Driver | 4 | 3 | Vehicle, Delivery |
| Accountant | 2 | 3 | Invoice, Payment, FinancialReport |
| SalesRepresentative | 3 | 3 | Customer, Contract, Order |
| QualityController | 2 | 3 | Batch, Recall |
| LogisticsManager | 2 | 3 | Route, Vehicle, Delivery |

### Транспорт

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| Vehicle | 7 | 4 | Driver, Delivery |
| Truck | 2 | 2 | Vehicle |
| Van | 2 | 1 | Vehicle |
| RefrigeratedTruck | 3 | 2 | Truck |
| Route | 6 | 3 | Address, Delivery |

### Документы

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| Document | 6 | 3 | — |
| Waybill | 4 | 3 | Shipment, Driver, Vehicle |
| Contract | 7 | 4 | Supplier, Customer, Manufacturer |
| PriceList | 4 | 3 | Medicine |
| Promotion | 6 | 3 | Medicine, Order |
| Discount | 5 | 2 | Customer, Order |
| PromotionCode | 5 | 3 | Discount |

### Отчётность

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| Report | 5 | 3 | Employee |
| SalesReport | 4 | 3 | Order, Medicine |
| StockReport | 4 | 3 | Warehouse, Batch |
| FinancialReport | 4 | 3 | Payment, Invoice |

### Безопасность и уведомления

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| User | 6 | 4 | Role, Employee, Customer |
| Role | 3 | 3 | Permission, User |
| Permission | 3 | 1 | Role |
| AuditLog | 5 | 3 | User |
| Notification | 6 | 3 | User, Customer, Employee |

### Качество и безопасность препаратов

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| Recall | 7 | 3 | Medicine, Batch, Customer |
| AdverseEvent | 7 | 3 | Medicine, Customer, Employee |
| ClinicalTrial | 7 | 3 | Medicine, Customer |

## Собственные классы исключений (20)

`PharmaException`, `MedicineNotFoundException`, `InvalidMedicineDataException`,
`PrescriptionRequiredException`, `ExpiredMedicineException`,
`InsufficientStockException`, `OrderNotFoundException`,
`InvalidOrderDataException`, `PaymentFailedException`,
`InsufficientFundsException`, `DeliveryFailedException`,
`VehicleNotAvailableException`, `DriverNotAvailableException`,
`WarehouseFullException`, `InvalidLicenseException`,
`CustomerNotFoundException`, `DuplicateCustomerException`,
`ColdChainViolationException`, `RecallException`, `AdverseEventException`.

## Итоговая статистика

| Показатель  | Значение |
|-------------|----------|
| Классы      | 68       |
| Поля        | 384      |
| Поведения   | 206      |
| Ассоциации  | 74       |
| Исключения  | 20       |