/**
 * @file test_main.cpp
 * @brief Unit-тесты для классов Vector и Polynomial (UnitTest++).
 * @author Ivan Kruk
 * @date 2026
 */

#include "UnitTest++/UnitTest++.h"
#include "Vector.h"
#include "Polynomial.h"

#include <sstream>
#include <stdexcept>
#include <cmath>

namespace {
    const double EPS = 1e-9;
}

//VECTOR

TEST(Vector_DefaultConstructor) {
    Vector v;
    CHECK_CLOSE(0.0, v.getX1(), EPS);
    CHECK_CLOSE(0.0, v.getZ2(), EPS);
}

TEST(Vector_ParamConstructor) {
    Vector v(1, 2, 3, 4, 5, 6);
    CHECK_CLOSE(1.0, v.getX1(), EPS);
    CHECK_CLOSE(6.0, v.getZ2(), EPS);
}

TEST(Vector_CopyConstructor) {
    Vector a(1, 2, 3, 4, 5, 6);
    Vector b(a);
    CHECK(a == b);
    CHECK_CLOSE(a.getX1(), b.getX1(), EPS);
    CHECK_CLOSE(a.getZ2(), b.getZ2(), EPS);
}

TEST(Vector_Assignment) {
    Vector a(1, 2, 3, 4, 5, 6);
    Vector b;
    b = a;
    CHECK_CLOSE(a.getX1(), b.getX1(), EPS);
    CHECK_CLOSE(a.getZ2(), b.getZ2(), EPS);
}

TEST(Vector_Setters) {
    Vector v;
    v.setX1(1); v.setY1(2); v.setZ1(3);
    v.setX2(4); v.setY2(5); v.setZ2(6);
    CHECK_CLOSE(1.0, v.getX1(), EPS);
    CHECK_CLOSE(6.0, v.getZ2(), EPS);
}

TEST(Vector_Length_Zero) {
    Vector v(1, 1, 1, 1, 1, 1);
    CHECK_CLOSE(0.0, v.length(), EPS);
}

TEST(Vector_Length_Unit) {
    Vector v(0, 0, 0, 1, 0, 0);
    CHECK_CLOSE(1.0, v.length(), EPS);
}

TEST(Vector_Length_345) {
    Vector v(0, 0, 0, 3, 4, 0);
    CHECK_CLOSE(5.0, v.length(), EPS);
}

TEST(Vector_Length_3D) {
    Vector v(0, 0, 0, 2, 3, 6);
    CHECK_CLOSE(7.0, v.length(), EPS);
}

TEST(Vector_Add) {
    Vector a(0, 0, 0, 1, 2, 3);
    Vector b(0, 0, 0, 4, 5, 6);
    Vector c = a + b;
    CHECK_CLOSE(5.0, c.getX2() - c.getX1(), EPS);
    CHECK_CLOSE(7.0, c.getY2() - c.getY1(), EPS);
    CHECK_CLOSE(9.0, c.getZ2() - c.getZ1(), EPS);
}

TEST(Vector_AddAssign) {
    Vector a(0, 0, 0, 1, 2, 3);
    Vector b(0, 0, 0, 4, 5, 6);
    a += b;
    CHECK_CLOSE(5.0, a.getX2() - a.getX1(), EPS);
    CHECK_CLOSE(7.0, a.getY2() - a.getY1(), EPS);
    CHECK_CLOSE(9.0, a.getZ2() - a.getZ1(), EPS);
}

TEST(Vector_Sub) {
    Vector a(0, 0, 0, 5, 5, 5);
    Vector b(0, 0, 0, 1, 2, 3);
    Vector c = a - b;
    CHECK_CLOSE(4.0, c.getX2() - c.getX1(), EPS);
    CHECK_CLOSE(3.0, c.getY2() - c.getY1(), EPS);
    CHECK_CLOSE(2.0, c.getZ2() - c.getZ1(), EPS);
}

TEST(Vector_SubAssign) {
    Vector a(0, 0, 0, 5, 5, 5);
    Vector b(0, 0, 0, 1, 2, 3);
    a -= b;
    CHECK_CLOSE(4.0, a.getX2() - a.getX1(), EPS);
}

TEST(Vector_CrossProduct_i_j_k) {
    Vector i(0, 0, 0, 1, 0, 0);
    Vector j(0, 0, 0, 0, 1, 0);
    Vector k = i * j;
    CHECK_CLOSE(0.0, k.getX2() - k.getX1(), EPS);
    CHECK_CLOSE(0.0, k.getY2() - k.getY1(), EPS);
    CHECK_CLOSE(1.0, k.getZ2() - k.getZ1(), EPS);
}

TEST(Vector_CrossProduct_Anticommutative) {
    Vector i(0, 0, 0, 1, 0, 0);
    Vector j(0, 0, 0, 0, 1, 0);
    Vector k1 = i * j;
    Vector k2 = j * i;
    CHECK_CLOSE(k1.getX2() - k1.getX1(), -(k2.getX2() - k2.getX1()), EPS);
    CHECK_CLOSE(k1.getY2() - k1.getY1(), -(k2.getY2() - k2.getY1()), EPS);
    CHECK_CLOSE(k1.getZ2() - k1.getZ1(), -(k2.getZ2() - k2.getZ1()), EPS);
}

TEST(Vector_CrossProductAssign) {
    Vector i(0, 0, 0, 1, 0, 0);
    Vector j(0, 0, 0, 0, 1, 0);
    i *= j;
    CHECK_CLOSE(0.0, i.getX2() - i.getX1(), EPS);
    CHECK_CLOSE(0.0, i.getY2() - i.getY1(), EPS);
    CHECK_CLOSE(1.0, i.getZ2() - i.getZ1(), EPS);
}

TEST(Vector_ScalarMultiply) {
    Vector a(0, 0, 0, 1, 2, 3);
    Vector b = a * 2.0;
    CHECK_CLOSE(2.0, b.getX2() - b.getX1(), EPS);
    CHECK_CLOSE(4.0, b.getY2() - b.getY1(), EPS);
    CHECK_CLOSE(6.0, b.getZ2() - b.getZ1(), EPS);
}

TEST(Vector_ScalarMultiplyAssign) {
    Vector a(0, 0, 0, 1, 2, 3);
    a *= 2.0;
    CHECK_CLOSE(2.0, a.getX2() - a.getX1(), EPS);
}

TEST(Vector_ScalarDivide) {
    Vector a(0, 0, 0, 2, 4, 6);
    Vector b = a / 2.0;
    CHECK_CLOSE(1.0, b.getX2() - b.getX1(), EPS);
    CHECK_CLOSE(2.0, b.getY2() - b.getY1(), EPS);
    CHECK_CLOSE(3.0, b.getZ2() - b.getZ1(), EPS);
}

TEST(Vector_ScalarDivideAssign) {
    Vector a(0, 0, 0, 2, 4, 6);
    a /= 2.0;
    CHECK_CLOSE(1.0, a.getX2() - a.getX1(), EPS);
}

TEST(Vector_DivideByZero_Throws) {
    Vector a(0, 0, 0, 2, 4, 6);
    CHECK_THROW(a / 0.0, std::invalid_argument);
}

TEST(Vector_Cosine_SameDirection) {
    Vector a(0, 0, 0, 1, 0, 0);
    Vector b(0, 0, 0, 2, 0, 0);
    CHECK_CLOSE(1.0, a ^ b, EPS);
}

TEST(Vector_Cosine_Perpendicular) {
    Vector a(0, 0, 0, 1, 0, 0);
    Vector b(0, 0, 0, 0, 1, 0);
    CHECK_CLOSE(0.0, a ^ b, EPS);
}

TEST(Vector_Cosine_Opposite) {
    Vector a(0, 0, 0, 1, 0, 0);
    Vector b(0, 0, 0, -1, 0, 0);
    CHECK_CLOSE(-1.0, a ^ b, EPS);
}

TEST(Vector_Cosine_45deg) {
    Vector a(0, 0, 0, 1, 0, 0);
    Vector b(0, 0, 0, 1, 1, 0);
    CHECK_CLOSE(std::sqrt(2.0) / 2.0, a ^ b, EPS);
}

TEST(Vector_Cosine_ZeroVector_Throws) {
    Vector a(0, 0, 0, 0, 0, 0);
    Vector b(0, 0, 0, 1, 0, 0);
    CHECK_THROW(a ^ b, std::invalid_argument);
}

TEST(Vector_Equality) {
    Vector a(0, 0, 0, 3, 4, 0);
    Vector b(0, 0, 0, 3, 4, 0);
    CHECK(a == b);
}

TEST(Vector_Inequality) {
    Vector a(0, 0, 0, 3, 4, 0);
    Vector b(0, 0, 0, 6, 8, 0);
    CHECK(a != b);
}

TEST(Vector_ComparisonOperators) {
    Vector a(0, 0, 0, 6, 8, 0);   // |a| = 10
    Vector b(0, 0, 0, 3, 4, 0);   // |b| = 5
    CHECK(a > b);
    CHECK(a >= b);
    CHECK(b < a);
    CHECK(b <= a);
    CHECK(a >= a);
    CHECK(a <= a);
}

TEST(Vector_StreamOutput) {
    Vector a(1, 2, 3, 4, 5, 6);
    std::ostringstream os;
    os << a;
    CHECK(!os.str().empty());
}

TEST(Vector_StreamInput) {
    Vector a;
    std::istringstream is("1 2 3 4 5 6");
    is >> a;
    CHECK_CLOSE(1.0, a.getX1(), EPS);
    CHECK_CLOSE(6.0, a.getZ2(), EPS);
}

//POLYNOMIAL

TEST(Poly_DefaultConstructor) {
    Polynomial p;
    CHECK_EQUAL(0u, p.getDegree());
    CHECK_CLOSE(0.0, p[0], EPS);
}

TEST(Poly_ArrayConstructor) {
    double coeffs[] = {1, 2, 3};
    Polynomial p(2, coeffs);
    CHECK_EQUAL(2u, p.getDegree());
    CHECK_CLOSE(1.0, p[0], EPS);
    CHECK_CLOSE(3.0, p[2], EPS);
}

TEST(Poly_InitListConstructor) {
    Polynomial p = {1, 2, 3};
    CHECK_EQUAL(2u, p.getDegree());
    CHECK_CLOSE(1.0, p[0], EPS);
    CHECK_CLOSE(3.0, p[2], EPS);
}

TEST(Poly_Normalization) {
    Polynomial p = {1, 2, 0, 0};
    CHECK_EQUAL(1u, p.getDegree());
}

TEST(Poly_CopyConstructor) {
    Polynomial a = {1, 2, 3};
    Polynomial b(a);
    CHECK(a == b);
}

TEST(Poly_Assignment) {
    Polynomial a = {1, 2, 3};
    Polynomial b;
    b = a;
    CHECK(a == b);
}

TEST(Poly_IndexReadWrite) {
    Polynomial p = {1, 2, 3};
    p[1] = 5.0;
    CHECK_CLOSE(5.0, p[1], EPS);
}

TEST(Poly_IndexOutOfRange) {
    Polynomial p = {1, 2, 3};
    CHECK_THROW(p[10], std::out_of_range);
}

TEST(Poly_IndexOutOfRange_Const) {
    const Polynomial p = {1, 2, 3};
    CHECK_THROW(p[10], std::out_of_range);
}

TEST(Poly_Evaluate) {
    // p(x) = 1 + 2x + 3x^2
    Polynomial p = {1, 2, 3};
    CHECK_CLOSE(1.0, p(0.0), EPS);
    CHECK_CLOSE(6.0, p(1.0), EPS);
    CHECK_CLOSE(17.0, p(2.0), EPS);
}

TEST(Poly_Evaluate_Negative) {
    Polynomial p = {1, 1}; // 1 + x
    CHECK_CLOSE(0.0, p(-1.0), EPS);
}

TEST(Poly_Add) {
    Polynomial a = {1, 2};
    Polynomial b = {3, 4, 5};
    Polynomial c = a + b;
    CHECK_CLOSE(4.0, c[0], EPS);
    CHECK_CLOSE(6.0, c[1], EPS);
    CHECK_CLOSE(5.0, c[2], EPS);
}

TEST(Poly_AddAssign) {
    Polynomial a = {1, 2};
    Polynomial b = {3, 4, 5};
    a += b;
    CHECK_CLOSE(4.0, a[0], EPS);
    CHECK_CLOSE(6.0, a[1], EPS);
    CHECK_CLOSE(5.0, a[2], EPS);
}

TEST(Poly_Sub) {
    Polynomial a = {5, 6, 7};
    Polynomial b = {1, 2, 3};
    Polynomial c = a - b;
    CHECK_CLOSE(4.0, c[0], EPS);
    CHECK_CLOSE(4.0, c[1], EPS);
    CHECK_CLOSE(4.0, c[2], EPS);
}

TEST(Poly_SubAssign) {
    Polynomial a = {5, 6, 7};
    Polynomial b = {1, 2, 3};
    a -= b;
    CHECK_CLOSE(4.0, a[0], EPS);
}

TEST(Poly_Multiply) {
    Polynomial a = {1, 1};
    Polynomial b = {1, 1};
    Polynomial c = a * b;
    CHECK_CLOSE(1.0, c[0], EPS);
    CHECK_CLOSE(2.0, c[1], EPS);
    CHECK_CLOSE(1.0, c[2], EPS);
}

TEST(Poly_MultiplyAssign) {
    Polynomial a = {1, 1};
    a *= Polynomial{1, 1};
    CHECK_CLOSE(1.0, a[0], EPS);
    CHECK_CLOSE(2.0, a[1], EPS);
    CHECK_CLOSE(1.0, a[2], EPS);
}

TEST(Poly_Divide_Exact) {
    // (x^2 - 1) / (x - 1) = x + 1
    Polynomial a = {-1, 0, 1};
    Polynomial b = {-1, 1};
    Polynomial c = a / b;
    CHECK_CLOSE(1.0, c[0], EPS);
    CHECK_CLOSE(1.0, c[1], EPS);
    CHECK_EQUAL(1u, c.getDegree());
}

TEST(Poly_Divide_WithRemainder_Throws) {
    Polynomial a = {1, 2, 3};
    Polynomial b = {1, 1};
    CHECK_THROW(a / b, std::invalid_argument);
}

TEST(Poly_Divide_ByZero_Throws) {
    Polynomial a = {1, 2, 3};
    Polynomial zero = {0};
    CHECK_THROW(a / zero, std::invalid_argument);
}

TEST(Poly_DivideAssign) {
    Polynomial a = {-1, 0, 1};
    Polynomial b = {-1, 1};
    a /= b;
    CHECK_CLOSE(1.0, a[0], EPS);
    CHECK_CLOSE(1.0, a[1], EPS);
}

TEST(Poly_Equal) {
    Polynomial a = {1, 2, 3};
    Polynomial b = {1, 2, 3};
    CHECK(a == b);
}

TEST(Poly_NotEqual) {
    Polynomial a = {1, 2, 3};
    Polynomial b = {1, 2, 4};
    CHECK(a != b);
}

TEST(Poly_StreamOutput) {
    Polynomial p = {1, 2, 3};
    std::ostringstream os;
    os << p;
    CHECK(!os.str().empty());
}

TEST(Poly_StreamOutput_Zero) {
    Polynomial p;
    std::ostringstream os;
    os << p;
    CHECK_EQUAL("0", os.str());
}

TEST(Poly_StreamInput) {
    Polynomial p;
    std::istringstream is("2 1 2 3");
    is >> p;
    CHECK_EQUAL(2u, p.getDegree());
    CHECK_CLOSE(1.0, p[0], EPS);
    CHECK_CLOSE(3.0, p[2], EPS);
}

int main() {
    return UnitTest::RunAllTests();
}
