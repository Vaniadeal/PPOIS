/**
 * @file main.cpp
 * @brief Консольное приложение с меню для демонстрации классов Vector и Polynomial.
 * @author Ivan Kruk
 * @date 2026
 */

#include "Vector.h"
#include "Polynomial.h"

#include <iostream>
#include <limits>
#include <vector>

namespace {

    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int readInt(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clearInput();
                return value;
            }
            std::cout << "Ошибка ввода. Попробуйте снова.\n";
            clearInput();
        }
    }

    double readDouble(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clearInput();
                return value;
            }
            std::cout << "Ошибка ввода. Попробуйте снова.\n";
            clearInput();
        }
    }

    Vector readVector(const std::string& name) {
        std::cout << "Введите координаты вектора " << name
                  << " (x1 y1 z1 x2 y2 z2): ";
        Vector v;
        std::cin >> v;
        clearInput();
        return v;
    }

    Polynomial readPolynomial(const std::string& name) {
        std::cout << "Введите степень многочлена " << name << ": ";
        int degree;
        std::cin >> degree;
        if (degree < 0) degree = 0;

        std::vector<double> coeffs(degree + 1);
        std::cout << "Введите " << (degree + 1)
                  << " коэффициентов от младшего к старшему:\n";
        for (int i = 0; i <= degree; ++i) {
            std::cout << "  coeff[" << i << "] = ";
            std::cin >> coeffs[i];
        }
        clearInput();
        return Polynomial(degree, coeffs.data());
    }

    //Меню вектора
    void vectorMenu() {
        Vector a, b;
        bool hasA = false, hasB = false;

        while (true) {
            std::cout << "\n    ВЕКТОР  \n";
            std::cout << "1. Задать вектор A\n";
            std::cout << "2. Задать вектор B\n";
            std::cout << "3. Показать A и B\n";
            std::cout << "4. Длина A\n";
            std::cout << "5. A + B\n";
            std::cout << "6. A - B\n";
            std::cout << "7. A * B (векторное)\n";
            std::cout << "8. A * число\n";
            std::cout << "9. A / число\n";
            std::cout << "10. Косинус угла между A и B\n";
            std::cout << "11. Сравнить A и B по длине\n";
            std::cout << "0. Назад\n";
            int choice = readInt("Выбор: ");

            if (choice == 0) return;

            try {
                switch (choice) {
                    case 1: a = readVector("A"); hasA = true; break;
                    case 2: b = readVector("B"); hasB = true; break;
                    case 3:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба вектора.\n"; break; }
                        std::cout << "A = " << a << "\nB = " << b << "\n";
                        break;
                    case 4:
                        if (!hasA) { std::cout << "Сначала задайте A.\n"; break; }
                        std::cout << "|A| = " << a.length() << "\n";
                        break;
                    case 5:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба вектора.\n"; break; }
                        std::cout << "A + B = " << (a + b) << "\n";
                        break;
                    case 6:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба вектора.\n"; break; }
                        std::cout << "A - B = " << (a - b) << "\n";
                        break;
                    case 7:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба вектора.\n"; break; }
                        std::cout << "A * B = " << (a * b) << "\n";
                        break;
                    case 8: {
                        if (!hasA) { std::cout << "Сначала задайте A.\n"; break; }
                        double k = readDouble("Введите число: ");
                        std::cout << "A * " << k << " = " << (a * k) << "\n";
                        break;
                    }
                    case 9: {
                        if (!hasA) { std::cout << "Сначала задайте A.\n"; break; }
                        double k = readDouble("Введите число (не 0): ");
                        std::cout << "A / " << k << " = " << (a / k) << "\n";
                        break;
                    }
                    case 10:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба вектора.\n"; break; }
                        std::cout << "cos(A, B) = " << (a ^ b) << "\n";
                        break;
                    case 11:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба вектора.\n"; break; }
                        if (a == b)      std::cout << "|A| == |B|\n";
                        else if (a > b)  std::cout << "|A| > |B|\n";
                        else             std::cout << "|A| < |B|\n";
                        break;
                    default:
                        std::cout << "Неверный пункт меню.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }
    }

    // Меню многочлена
    void polynomialMenu() {
        Polynomial a, b;
        bool hasA = false, hasB = false;

        while (true) {
            std::cout << "\n    МНОГОЧЛЕН   \n";
            std::cout << "1. Задать многочлен A\n";
            std::cout << "2. Задать многочлен B\n";
            std::cout << "3. Показать A и B\n";
            std::cout << "4. Значение A(x) в точке\n";
            std::cout << "5. A + B\n";
            std::cout << "6. A - B\n";
            std::cout << "7. A * B\n";
            std::cout << "8. A / B\n";
            std::cout << "9. Сравнить A и B\n";
            std::cout << "0. Назад\n";
            int choice = readInt("Выбор: ");

            if (choice == 0) return;

            try {
                switch (choice) {
                    case 1: a = readPolynomial("A"); hasA = true; break;
                    case 2: b = readPolynomial("B"); hasB = true; break;
                    case 3:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба многочлена.\n"; break; }
                        std::cout << "A = " << a << "\nB = " << b << "\n";
                        break;
                    case 4: {
                        if (!hasA) { std::cout << "Сначала задайте A.\n"; break; }
                        double x = readDouble("Введите x: ");
                        std::cout << "A(" << x << ") = " << a(x) << "\n";
                        break;
                    }
                    case 5:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба многочлена.\n"; break; }
                        std::cout << "A + B = " << (a + b) << "\n";
                        break;
                    case 6:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба многочлена.\n"; break; }
                        std::cout << "A - B = " << (a - b) << "\n";
                        break;
                    case 7:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба многочлена.\n"; break; }
                        std::cout << "A * B = " << (a * b) << "\n";
                        break;
                    case 8:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба многочлена.\n"; break; }
                        std::cout << "A / B = " << (a / b) << "\n";
                        break;
                    case 9:
                        if (!hasA || !hasB) { std::cout << "Сначала задайте оба многочлена.\n"; break; }
                        std::cout << (a == b ? "A == B\n" : "A != B\n");
                        break;
                    default:
                        std::cout << "Неверный пункт меню.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }
    }

} // namespace

int main() {
    std::cout << "Лабораторная работа №1. PPOIS.\n";

    while (true) {
        std::cout << "\n    ГЛАВНОЕ МЕНЮ    \n";
        std::cout << "1. Работа с вектором\n";
        std::cout << "2. Работа с многочленом\n";
        std::cout << "0. Выход\n";
        int choice = readInt("Выбор: ");

        switch (choice) {
            case 1: vectorMenu(); break;
            case 2: polynomialMenu(); break;
            case 0:
                std::cout << "Выход.\n";
                return 0;
            default:
                std::cout << "Неверный пункт меню.\n";
        }
    }
}
