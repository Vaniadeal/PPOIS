/**
 * @file Polynomial.h
 * @brief Объявление класса Polynomial — многочлен от одной переменной.
 * @author Ivan Kruk
 * @date 2026
 */

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <cstddef>
#include <initializer_list>

/**
 * @brief Класс многочлена от одной переменной.
 *
 * Многочлен задается массивом коэффициентов: coeffs_[i] — коэффициент при x^i.
 * Старшие нулевые коэффициенты отбрасываются (нормализация).
 */
class Polynomial {
private:
    std::size_t size_;  ///< Количество коэффициентов (degree + 1)
    double* coeffs_;    ///< Массив коэффициентов

    /// @brief Убирает ведущие нулевые коэффициенты.
    void normalize();

public:
    /**
     * @brief Конструктор по умолчанию — нулевой многочлен.
     */
    Polynomial();

    /**
     * @brief Конструктор по степени и массиву коэффициентов.
     * @param degree Степень многочлена
     * @param coeffs Массив длины degree + 1 (coeffs[i] — при x^i)
     */
    Polynomial(std::size_t degree, const double* coeffs);

    /**
     * @brief Удобный конструктор из списка инициализации.
     * @param coeffs Коэффициенты от младшего к старшему
     */
    Polynomial(std::initializer_list<double> coeffs);

    /**
     * @brief Конструктор копирования.
     */
    Polynomial(const Polynomial& other);

    /**
     * @brief Оператор присваивания.
     */
    Polynomial& operator=(const Polynomial& other);

    /**
     * @brief Деструктор. Освобождает массив коэффициентов.
     */
    ~Polynomial();

    /**
     * @brief Получить степень многочлена.
     */
    std::size_t getDegree() const;

    /**
     * @brief Доступ к коэффициенту по индексу (чтение).
     * @throw std::out_of_range если индекс вне диапазона
     */
    const double& operator[](std::size_t index) const;

    /**
     * @brief Доступ к коэффициенту по индексу (запись).
     * @throw std::out_of_range если индекс вне диапазона
     */
    double& operator[](std::size_t index);

    /**
     * @brief Вычислить значение многочлена в точке x (схема Горнера).
     */
    double operator()(double x) const;

    // Арифметика 
    Polynomial operator+(const Polynomial& other) const;
    Polynomial& operator+=(const Polynomial& other);

    Polynomial operator-(const Polynomial& other) const;
    Polynomial& operator-=(const Polynomial& other);

    Polynomial operator*(const Polynomial& other) const;
    Polynomial& operator*=(const Polynomial& other);

    /**
     * @brief Деление многочленов нацело.
     * @throw std::invalid_argument если делитель нулевой или деление не нацело
     */
    Polynomial operator/(const Polynomial& other) const;
    Polynomial& operator/=(const Polynomial& other);

    // Сравнение
    bool operator==(const Polynomial& other) const;
    bool operator!=(const Polynomial& other) const;

    // Потоки
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);
    friend std::istream& operator>>(std::istream& is, Polynomial& p);
};

#endif // POLYNOMIAL_H
