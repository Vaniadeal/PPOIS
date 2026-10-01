/**
 * @file main.cpp
 * @brief Console application with a menu to demonstrate the Vector and Polynomial classes.
 * @author Крюк Иван Витальевич
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
            std::cout << "Input error. Please try again.\n";
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
            std::cout << "Input error. Please try again.\n";
            clearInput();
        }
    }

    Vector readVector(const std::string& name) {
        std::cout << "Enter coordinates of vector " << name
                  << " (x1 y1 z1 x2 y2 z2): ";
        Vector v;
        std::cin >> v;
        clearInput();
        return v;
    }

    Polynomial readPolynomial(const std::string& name) {
        std::cout << "Enter degree of polynomial " << name << ": ";
        int degree;
        std::cin >> degree;
        if (degree < 0) degree = 0;

        std::vector<double> coeffs(degree + 1);
        std::cout << "Enter " << (degree + 1)
                  << " coefficients from lowest to highest:\n";
        for (int i = 0; i <= degree; ++i) {
            std::cout << "  coeff[" << i << "] = ";
            std::cin >> coeffs[i];
        }
        clearInput();
        return Polynomial(degree, coeffs.data());
    }

    // Vector menu 
    void vectorMenu() {
        Vector a, b;
        bool hasA = false, hasB = false;

        while (true) {
            std::cout << "\n===== VECTOR =====\n";
            std::cout << "1. Set vector A\n";
            std::cout << "2. Set vector B\n";
            std::cout << "3. Show A and B\n";
            std::cout << "4. Length of A\n";
            std::cout << "5. A + B\n";
            std::cout << "6. A - B\n";
            std::cout << "7. A * B (cross product)\n";
            std::cout << "8. A * scalar\n";
            std::cout << "9. A / scalar\n";
            std::cout << "10. Cosine of angle between A and B\n";
            std::cout << "11. Compare A and B by length\n";
            std::cout << "0. Back\n";
            int choice = readInt("Choice: ");

            if (choice == 0) return;

            try {
                switch (choice) {
                    case 1: a = readVector("A"); hasA = true; break;
                    case 2: b = readVector("B"); hasB = true; break;
                    case 3:
                        if (!hasA || !hasB) { std::cout << "Set both vectors first.\n"; break; }
                        std::cout << "A = " << a << "\nB = " << b << "\n";
                        break;
                    case 4:
                        if (!hasA) { std::cout << "Set vector A first.\n"; break; }
                        std::cout << "|A| = " << a.length() << "\n";
                        break;
                    case 5:
                        if (!hasA || !hasB) { std::cout << "Set both vectors first.\n"; break; }
                        std::cout << "A + B = " << (a + b) << "\n";
                        break;
                    case 6:
                        if (!hasA || !hasB) { std::cout << "Set both vectors first.\n"; break; }
                        std::cout << "A - B = " << (a - b) << "\n";
                        break;
                    case 7:
                        if (!hasA || !hasB) { std::cout << "Set both vectors first.\n"; break; }
                        std::cout << "A * B = " << (a * b) << "\n";
                        break;
                    case 8: {
                        if (!hasA) { std::cout << "Set vector A first.\n"; break; }
                        double k = readDouble("Enter scalar: ");
                        std::cout << "A * " << k << " = " << (a * k) << "\n";
                        break;
                    }
                    case 9: {
                        if (!hasA) { std::cout << "Set vector A first.\n"; break; }
                        double k = readDouble("Enter scalar (non-zero): ");
                        std::cout << "A / " << k << " = " << (a / k) << "\n";
                        break;
                    }
                    case 10:
                        if (!hasA || !hasB) { std::cout << "Set both vectors first.\n"; break; }
                        std::cout << "cos(A, B) = " << (a ^ b) << "\n";
                        break;
                    case 11:
                        if (!hasA || !hasB) { std::cout << "Set both vectors first.\n"; break; }
                        if (a == b)      std::cout << "|A| == |B|\n";
                        else if (a > b)  std::cout << "|A| > |B|\n";
                        else             std::cout << "|A| < |B|\n";
                        break;
                    default:
                        std::cout << "Invalid menu item.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }

    //Polynomial menu
    void polynomialMenu() {
        Polynomial a, b;
        bool hasA = false, hasB = false;

        while (true) {
            std::cout << "\n===== POLYNOMIAL =====\n";
            std::cout << "1. Set polynomial A\n";
            std::cout << "2. Set polynomial B\n";
            std::cout << "3. Show A and B\n";
            std::cout << "4. Value of A(x) at a point\n";
            std::cout << "5. A + B\n";
            std::cout << "6. A - B\n";
            std::cout << "7. A * B\n";
            std::cout << "8. A / B\n";
            std::cout << "9. Compare A and B\n";
            std::cout << "0. Back\n";
            int choice = readInt("Choice: ");

            if (choice == 0) return;

            try {
                switch (choice) {
                    case 1: a = readPolynomial("A"); hasA = true; break;
                    case 2: b = readPolynomial("B"); hasB = true; break;
                    case 3:
                        if (!hasA || !hasB) { std::cout << "Set both polynomials first.\n"; break; }
                        std::cout << "A = " << a << "\nB = " << b << "\n";
                        break;
                    case 4: {
                        if (!hasA) { std::cout << "Set polynomial A first.\n"; break; }
                        double x = readDouble("Enter x: ");
                        std::cout << "A(" << x << ") = " << a(x) << "\n";
                        break;
                    }
                    case 5:
                        if (!hasA || !hasB) { std::cout << "Set both polynomials first.\n"; break; }
                        std::cout << "A + B = " << (a + b) << "\n";
                        break;
                    case 6:
                        if (!hasA || !hasB) { std::cout << "Set both polynomials first.\n"; break; }
                        std::cout << "A - B = " << (a - b) << "\n";
                        break;
                    case 7:
                        if (!hasA || !hasB) { std::cout << "Set both polynomials first.\n"; break; }
                        std::cout << "A * B = " << (a * b) << "\n";
                        break;
                    case 8:
                        if (!hasA || !hasB) { std::cout << "Set both polynomials first.\n"; break; }
                        std::cout << "A / B = " << (a / b) << "\n";
                        break;
                    case 9:
                        if (!hasA || !hasB) { std::cout << "Set both polynomials first.\n"; break; }
                        std::cout << (a == b ? "A == B\n" : "A != B\n");
                        break;
                    default:
                        std::cout << "Invalid menu item.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }

} // namespace

int main() {
    std::cout << "Laboratory work #1. PPOIS.\n";

    while (true) {
        std::cout << "\n===== MAIN MENU =====\n";
        std::cout << "1. Work with Vector\n";
        std::cout << "2. Work with Polynomial\n";
        std::cout << "0. Exit\n";
        int choice = readInt("Choice: ");

        switch (choice) {
            case 1: vectorMenu(); break;
            case 2: polynomialMenu(); break;
            case 0:
                std::cout << "Exit.\n";
                return 0;
            default:
                std::cout << "Invalid menu item.\n";
        }
    }
}