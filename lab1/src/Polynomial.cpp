/**
 * @file Polynomial.cpp
 * @brief Реализация класса Polynomial.
 * @author Kruk Ivan
 * @date 2026
 */

#include "Polynomial.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace {
    const double EPS = 1e-9;

    bool isZero(double x) { return std::abs(x) < EPS; }
}

// Конструкторы / деструктор 

Polynomial::Polynomial()
    : size_(1), coeffs_(new double[1]{0.0}) {}

Polynomial::Polynomial(std::size_t degree, const double* coeffs)
    : size_(degree + 1), coeffs_(new double[degree + 1]) {
    for (std::size_t i = 0; i <= degree; ++i) {
        coeffs_[i] = coeffs[i];
    }
    normalize();
}

Polynomial::Polynomial(std::initializer_list<double> coeffs)
    : size_(coeffs.size() == 0 ? 1 : coeffs.size()),
      coeffs_(new double[coeffs.size() == 0 ? 1 : coeffs.size()]) {
    std::size_t i = 0;
    for (double c : coeffs) coeffs_[i++] = c;
    if (i == 0) coeffs_[0] = 0.0;
    normalize();
}

Polynomial::Polynomial(const Polynomial& other)
    : size_(other.size_), coeffs_(new double[other.size_]) {
    for (std::size_t i = 0; i < size_; ++i) coeffs_[i] = other.coeffs_[i];
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this != &other) {
        double* newCoeffs = new double[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) newCoeffs[i] = other.coeffs_[i];
        delete[] coeffs_;
        coeffs_ = newCoeffs;
        size_ = other.size_;
    }
    return *this;
}

Polynomial::~Polynomial() {
    delete[] coeffs_;
}

void Polynomial::normalize() {
    while (size_ > 1 && isZero(coeffs_[size_ - 1])) {
        --size_;
    }
}

// Геттеры / доступ

std::size_t Polynomial::getDegree() const {
    return size_ - 1;
}

const double& Polynomial::operator[](std::size_t index) const {
    if (index >= size_) throw std::out_of_range("Polynomial index out of range");
    return coeffs_[index];
}

double& Polynomial::operator[](std::size_t index) {
    if (index >= size_) throw std::out_of_range("Polynomial index out of range");
    return coeffs_[index];
}

double Polynomial::operator()(double x) const {
    double result = 0.0;
    for (std::size_t i = size_; i-- > 0; ) {
        result = result * x + coeffs_[i];
    }
    return result;
}

//Сложение 

Polynomial Polynomial::operator+(const Polynomial& other) const {
    std::size_t newSize = std::max(size_, other.size_);
    double* newCoeffs = new double[newSize]();
    for (std::size_t i = 0; i < size_; ++i)       newCoeffs[i] += coeffs_[i];
    for (std::size_t i = 0; i < other.size_; ++i) newCoeffs[i] += other.coeffs_[i];

    Polynomial result(newSize - 1, newCoeffs);
    delete[] newCoeffs;
    return result;
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
    *this = *this + other;
    return *this;
}

// Вычитание

Polynomial Polynomial::operator-(const Polynomial& other) const {
    std::size_t newSize = std::max(size_, other.size_);
    double* newCoeffs = new double[newSize]();
    for (std::size_t i = 0; i < size_; ++i)       newCoeffs[i] += coeffs_[i];
    for (std::size_t i = 0; i < other.size_; ++i) newCoeffs[i] -= other.coeffs_[i];

    Polynomial result(newSize - 1, newCoeffs);
    delete[] newCoeffs;
    return result;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
    *this = *this - other;
    return *this;
}

// Умножение

Polynomial Polynomial::operator*(const Polynomial& other) const {
    std::size_t newSize = size_ + other.size_ - 1;
    double* newCoeffs = new double[newSize]();
    for (std::size_t i = 0; i < size_; ++i) {
        for (std::size_t j = 0; j < other.size_; ++j) {
            newCoeffs[i + j] += coeffs_[i] * other.coeffs_[j];
        }
    }

    Polynomial result(newSize - 1, newCoeffs);
    delete[] newCoeffs;
    return result;
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {
    *this = *this * other;
    return *this;
}

//Деление

Polynomial Polynomial::operator/(const Polynomial& other) const {
    if (other.size_ == 1 && isZero(other.coeffs_[0])) {
        throw std::invalid_argument("Division by zero polynomial");
    }

    if (size_ < other.size_) {
        return Polynomial();
    }

    std::size_t quotientSize = size_ - other.size_ + 1;
    double* quotientCoeffs = new double[quotientSize]();

    std::size_t remSize = size_;
    double* remainder = new double[remSize];
    for (std::size_t i = 0; i < remSize; ++i) remainder[i] = coeffs_[i];

    while (remSize >= other.size_) {
        std::size_t shift = remSize - other.size_;
        double factor = remainder[remSize - 1] / other.coeffs_[other.size_ - 1];
        quotientCoeffs[shift] = factor;

        for (std::size_t i = 0; i < other.size_; ++i) {
            remainder[shift + i] -= factor * other.coeffs_[i];
        }

        while (remSize > 0 && isZero(remainder[remSize - 1])) {
            --remSize;
        }
    }

    bool hasRemainder = false;
    for (std::size_t i = 0; i < remSize; ++i) {
        if (!isZero(remainder[i])) { hasRemainder = true; break; }
    }

    if (hasRemainder) {
        delete[] quotientCoeffs;
        delete[] remainder;
        throw std::invalid_argument("Polynomials are not divisible (non-zero remainder)");
    }

    Polynomial result(quotientSize - 1, quotientCoeffs);
    delete[] quotientCoeffs;
    delete[] remainder;
    return result;
}

Polynomial& Polynomial::operator/=(const Polynomial& other) {
    *this = *this / other;
    return *this;
}

// Сравнение

bool Polynomial::operator==(const Polynomial& other) const {
    if (size_ != other.size_) return false;
    for (std::size_t i = 0; i < size_; ++i) {
        if (std::abs(coeffs_[i] - other.coeffs_[i]) > EPS) return false;
    }
    return true;
}

bool Polynomial::operator!=(const Polynomial& other) const {
    return !(*this == other);
}

// Потоки

std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
    bool printed = false;
    for (std::size_t i = p.size_; i-- > 0; ) {
        double c = p.coeffs_[i];
        if (isZero(c)) continue;

        double absC = std::abs(c);
        if (printed) {
            os << (c > 0 ? " + " : " - ");
        } else if (c < 0) {
            os << "-";
        }

        if (i == 0) {
            os << absC;
        } else {
            if (std::abs(absC - 1.0) > EPS) os << absC;
            os << "x";
            if (i > 1) os << "^" << i;
        }
        printed = true;
    }
    if (!printed) os << "0";
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& p) {
    std::size_t degree;
    is >> degree;

    double* coeffs = new double[degree + 1];
    for (std::size_t i = 0; i <= degree; ++i) {
        is >> coeffs[i];
    }

    Polynomial temp(degree, coeffs);
    delete[] coeffs;
    p = temp;
    return is;
}
