#ifndef VECTOR4_H
#define VECTOR4_H
#include <cstdio>
#include <cmath>

// #include "Normal3.h"
// class Normal3;
class Vector3;

class Vector4
{

public:
    double m_x;
    double m_y;
    double m_z;
    double m_w;

    // default constructor
    Vector4(void);
    // constructor
    Vector4(const double a);
    // constructor
    Vector4(const double x, const double y, const double z, const double w);
    // copy constructor
    Vector4(const Vector3 &v);
    // deconstructor
    ~Vector4();
    Vector4 operator+(const Vector4 &v2) const; // v + v2
    // Vector4 operator+(const Normal3 &n) const;  // v + normal
    Vector4 operator-(const Vector4 &v2) const; // v - v2
    Vector4 operator-() const;                  // -v
    Vector4 operator*(const double a) const;    // v * a
    double operator*(const Vector4 &v2) const;  // v * v2
    // double operator*(const Normal3 &n) const;   // v * normal
    // Vector4 operator^(const Vector4 &v2) const; // v ^ v2
    Vector4 operator/(const double a) const;    // v / a
    Vector4 &operator=(const Vector4 &rhs);     // assignment operator
    Vector4 &operator+=(const Vector4 &rhs);    // += operator
    double len(void) const;                     // ||v||
    double len_squared(void) const;             // ||v||^2
    void normalize(void);
    Vector4 &hat(void);
    void prettyPrint(void) const;
};

// v + v2
inline Vector4 Vector4::operator+(const Vector4 &v2) const
{
    if (!((v2.m_w) && m_w)) // allow addition if not both are points
        return Vector4(m_x + v2.m_x, m_y + v2.m_y, m_z + v2.m_z, v2.m_w + m_w);

    else
    {
        fprintf(stderr, "Cannot add Points!\n");
        exit(EXIT_FAILURE);
    }
}

// v - v2
inline Vector4 Vector4::operator-(const Vector4 &v2) const
{
    if (!((v2.m_w) && m_w)) // allow addition if not both are points
        return Vector4(m_x - v2.m_x, m_y - v2.m_y, m_z - v2.m_z, v2.m_w + m_w);

    else
    {
        fprintf(stderr, "Cannot subtract Points!\n");
        exit(EXIT_FAILURE);
    }
}
// -v
inline Vector4 Vector4::operator-() const
{
    return (Vector4(-m_x, -m_y, -m_z, m_w));
}
// v * a
inline Vector4 Vector4::operator*(const double a) const
{
    return Vector4(m_x * a, m_y * a, m_z * a, m_w * a);
}
// a * v
Vector4 operator*(const double a, const Vector4 &v);
inline Vector4 operator*(const double a, const Vector4 &v)
{
    return Vector4(v.m_x * a, v.m_y * a, v.m_z * a, v.m_w * a);
}

// v / a
inline Vector4 Vector4::operator/(const double a) const
{
    return Vector4(m_x / a, m_y / a, m_z / a, m_w / a);
}
// ||v||
inline double Vector4::len(void) const
{
    return (sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w));
}

// ||v||^2
inline double Vector4::len_squared(void) const
{
    return (m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

// v * v2 -- dot product
inline double Vector4::operator*(const Vector4 &v2) const
{
    return (m_x * v2.m_x + m_y * v2.m_y + m_z * v2.m_z + m_w * v2.m_w);
}

// // v ^ v2 -- cross product
// inline Vector4 Vector4::operator^(const Vector4 &v2) const
// {
//     return (Vector4(m_y * v2.m_z - m_z * v2.m_y,
//                     m_z * v2.m_x - m_x * v2.m_z,
//                     m_x * v2.m_y - m_y * v2.m_x));
// }

// += operator
inline Vector4 &Vector4::operator+=(const Vector4 &rhs)
{
    m_x += rhs.m_x;
    m_y += rhs.m_y;
    m_z += rhs.m_z;
    m_w += rhs.m_w;
    return (*this);
}

inline void Vector4::prettyPrint(void) const
{
    fprintf(stdout, "[ %f , %f , %f , %f ]\n", m_x, m_y, m_z,m_w);
}

#endif