/**
 * @file Vector.h
 * @brief Объявление класса Vector для работы с векторами в 3D пространстве.
 * @author Ivan Kruk
 * @date 2026
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

/**
 * @brief Класс, представляющий вектор в трехмерном пространстве.
 *
 * Вектор задается координатами двух концов: начала и конца.
 * Поддерживает арифметические операции, сравнение, ввод/вывод.
 */
class Vector {
private:
    double x1_, y1_, z1_; ///< Координаты начала вектора
    double x2_, y2_, z2_; ///< Координаты конца вектора

public:
    /**
     * @brief Конструктор по умолчанию. Создает нулевой вектор.
     */
    Vector();

    /**
     * @brief Конструктор с параметрами.
     * @param x1 X начала
     * @param y1 Y начала
     * @param z1 Z начала
     * @param x2 X конца
     * @param y2 Y конца
     * @param z2 Z конца
     */
    Vector(double x1, double y1, double z1,
           double x2, double y2, double z2);

    /**
     * @brief Конструктор копирования.
     * @param other Копируемый вектор
     */
    Vector(const Vector& other);

    /**
     * @brief Оператор присваивания.
     * @param other Присваиваемый вектор
     * @return Ссылка на текущий объект
     */
    Vector& operator=(const Vector& other);

    /**
     * @brief Деструктор.
     */
    ~Vector();

    // Геттеры
    double getX1() const;
    double getY1() const;
    double getZ1() const;
    double getX2() const;
    double getY2() const;
    double getZ2() const;

    // Сеттеры
    void setX1(double x);
    void setY1(double y);
    void setZ1(double z);
    void setX2(double x);
    void setY2(double y);
    void setZ2(double z);

    /**
     * @brief Вычисление длины вектора.
     * @return Длина вектора
     */
    double length() const;

    // Операторы сравнения (по длине)
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
    bool operator>(const Vector& other) const;
    bool operator>=(const Vector& other) const;
    bool operator<(const Vector& other) const;
    bool operator<=(const Vector& other) const;

    //  Арифметические операторы 
    Vector operator+(const Vector& other) const;
    Vector& operator+=(const Vector& other);

    Vector operator-(const Vector& other) const;
    Vector& operator-=(const Vector& other);

    /// @brief Векторное произведение
    Vector operator*(const Vector& other) const;
    Vector& operator*=(const Vector& other);

    /// @brief Умножение на скаляр
    Vector operator*(double scalar) const;
    Vector& operator*=(double scalar);

    /// @brief Поэлементное деление векторов
    Vector operator/(double scalar) const;
    Vector& operator/=(double scalar);

    /**
     * @brief Косинус угла между векторами.
     * @param other Второй вектор
     * @return Значение косинуса
     */
    double operator^(const Vector& other) const;

    // Потоковые операторы
    friend std::ostream& operator<<(std::ostream& os, const Vector& v);
    friend std::istream& operator>>(std::istream& is, Vector& v);
};

#endif // VECTOR_H
