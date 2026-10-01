/**
 * @file Vector.cpp
 * @brief Реализация класса Vector.
 * @author Ivan Kruk
 * @date 2026
 */

#include "Vector.h"
#include <cmath>
#include <stdexcept>

// Конструкторы / деструктор 

Vector::Vector()
    : x1_(0), y1_(0), z1_(0), x2_(0), y2_(0), z2_(0) {}

Vector::Vector(double x1, double y1, double z1,
               double x2, double y2, double z2)
    : x1_(x1), y1_(y1), z1_(z1), x2_(x2), y2_(y2), z2_(z2) {}

Vector::Vector(const Vector& other)
    : x1_(other.x1_), y1_(other.y1_), z1_(other.z1_),
      x2_(other.x2_), y2_(other.y2_), z2_(other.z2_) {}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        x1_ = other.x1_; y1_ = other.y1_; z1_ = other.z1_;
        x2_ = other.x2_; y2_ = other.y2_; z2_ = other.z2_;
    }
    return *this;
}

Vector::~Vector() = default;

// Геттеры 

double Vector::getX1() const { return x1_; }
double Vector::getY1() const { return y1_; }
double Vector::getZ1() const { return z1_; }
double Vector::getX2() const { return x2_; }
double Vector::getY2() const { return y2_; }
double Vector::getZ2() const { return z2_; }

// Сеттеры 

void Vector::setX1(double x) { x1_ = x; }
void Vector::setY1(double y) { y1_ = y; }
void Vector::setZ1(double z) { z1_ = z; }
void Vector::setX2(double x) { x2_ = x; }
void Vector::setY2(double y) { y2_ = y; }
void Vector::setZ2(double z) { z2_ = z; }

// Вспомогательные 
namespace {
    double dx(const Vector& v) { return v.getX2() - v.getX1(); }
    double dy(const Vector& v) { return v.getY2() - v.getY1(); }
    double dz(const Vector& v) { return v.getZ2() - v.getZ1(); }

    /// Создает вектор из начала координат в точку (x, y, z)
    Vector makeFromOrigin(double x, double y, double z) {
        return Vector(0, 0, 0, x, y, z);
    }
}

// Длина 

double Vector::length() const {
    return std::sqrt(dx(*this) * dx(*this) +
                     dy(*this) * dy(*this) +
                     dz(*this) * dz(*this));
}

// Сравнение (по длине)

bool Vector::operator==(const Vector& other) const {
    return std::abs(length() - other.length()) < 1e-9;
}
bool Vector::operator!=(const Vector& other) const { return !(*this == other); }
bool Vector::operator>(const Vector& other) const  { return length() > other.length(); }
bool Vector::operator>=(const Vector& other) const { return length() >= other.length(); }
bool Vector::operator<(const Vector& other) const  { return length() < other.length(); }
bool Vector::operator<=(const Vector& other) const { return length() <= other.length(); }

// Сложение

Vector Vector::operator+(const Vector& other) const {
    return makeFromOrigin(dx(*this) + dx(other),
                          dy(*this) + dy(other),
                          dz(*this) + dz(other));
}

Vector& Vector::operator+=(const Vector& other) {
    x2_ += dx(other);
    y2_ += dy(other);
    z2_ += dz(other);
    return *this;
}

// Вычитание

Vector Vector::operator-(const Vector& other) const {
    return makeFromOrigin(dx(*this) - dx(other),
                          dy(*this) - dy(other),
                          dz(*this) - dz(other));
}

Vector& Vector::operator-=(const Vector& other) {
    x2_ -= dx(other);
    y2_ -= dy(other);
    z2_ -= dz(other);
    return *this;
}

// Векторное произведение

Vector Vector::operator*(const Vector& other) const {
    double a = dy(*this) * dz(other) - dz(*this) * dy(other);
    double b = dz(*this) * dx(other) - dx(*this) * dz(other);
    double c = dx(*this) * dy(other) - dy(*this) * dx(other);
    return makeFromOrigin(a, b, c);
}

Vector& Vector::operator*=(const Vector& other) {
    *this = *this * other;
    return *this;
}

// Умножение на скаляр

Vector Vector::operator*(double scalar) const {
    return makeFromOrigin(dx(*this) * scalar,
                          dy(*this) * scalar,
                          dz(*this) * scalar);
}

Vector& Vector::operator*=(double scalar) {
    x2_ = x1_ + dx(*this) * scalar;
    y2_ = y1_ + dy(*this) * scalar;
    z2_ = z1_ + dz(*this) * scalar;
    return *this;
}

// Деление на скаляр

Vector Vector::operator/(double scalar) const {
    if (std::abs(scalar) < 1e-9) {
        throw std::invalid_argument("Vector division by zero scalar");
    }
    return makeFromOrigin(dx(*this) / scalar,
                          dy(*this) / scalar,
                          dz(*this) / scalar);
}

Vector& Vector::operator/=(double scalar) {
    if (std::abs(scalar) < 1e-9) {
        throw std::invalid_argument("Vector division by zero scalar");
    }
    x2_ = x1_ + dx(*this) / scalar;
    y2_ = y1_ + dy(*this) / scalar;
    z2_ = z1_ + dz(*this) / scalar;
    return *this;
}

// Косинус угла

double Vector::operator^(const Vector& other) const {
    double dot = dx(*this) * dx(other) +
                 dy(*this) * dy(other) +
                 dz(*this) * dz(other);
    double lenProduct = length() * other.length();
    if (lenProduct < 1e-9) {
        throw std::invalid_argument("Cannot compute angle with zero-length vector");
    }
    return dot / lenProduct;
}

// Потоковые операторы

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << "(" << v.x1_ << ", " << v.y1_ << ", " << v.z1_ << ") -> ("
       << v.x2_ << ", " << v.y2_ << ", " << v.z2_ << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Vector& v) {
    is >> v.x1_ >> v.y1_ >> v.z1_
       >> v.x2_ >> v.y2_ >> v.z2_;
    return is;
}
