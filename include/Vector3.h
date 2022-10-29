#ifndef VECTOR3_H
#define VECTOR3_H
#include <cstdio>
#include <cmath>

// #include "Normal3.h"
class Normal3;

class Vector3
{

public:
    double m_x;
    double m_y;
    double m_z;

    // default constructor
    Vector3(void);
    // constructor
    Vector3(const double a);
    // constructor
    Vector3(const double x, const double y, const double z);
    // copy constructor
    Vector3(const Vector3 &v);
    // deconstructor
    ~Vector3();
    Vector3 operator+(const Vector3 &v2) const; // v + v2
    Vector3 operator+(const Normal3 &n) const;  // v + normal
    Vector3 operator-(const Vector3 &v2) const; // v - v2
    Vector3 operator-() const;                  // -v
    Vector3 operator*(const double a) const;    // v * a
    double operator*(const Vector3 &v2) const;  // v * v2
    double operator*(const Normal3 &n) const;   // v * normal
    Vector3 operator^(const Vector3 &v2) const; // v ^ v2
    Vector3 operator/(const double a) const;    // v / a
    Vector3 &operator=(const Vector3 &rhs);     // assignment operator
    Vector3 &operator+=(const Vector3 &rhs);    // += operator
    double len(void) const;                     // ||v||
    double len_squared(void) const;             // ||v||^2
    void normalize(void);
    Vector3 &hat(void);
    void prettyPrint(void) const;
};

// v + v2
inline Vector3 Vector3::operator+(const Vector3 &v2) const
{
    return Vector3(m_x + v2.m_x, m_y + v2.m_y, m_z + v2.m_z);
}

// v - v2
inline Vector3 Vector3::operator-(const Vector3 &v2) const
{
    return Vector3(m_x - v2.m_x, m_y - v2.m_y, m_z - v2.m_z);
}
// -v
inline Vector3 Vector3::operator-() const
{
    return (Vector3(-m_x, -m_y, -m_z));
}
// v * a
inline Vector3 Vector3::operator*(const double a) const
{
    return Vector3(m_x * a, m_y * a, m_z * a);
}
// a * v
Vector3 operator*(const double a, const Vector3 &v);
inline Vector3 operator*(const double a, const Vector3 &v)
{
    return Vector3(v.m_x * a, v.m_y * a, v.m_z * a);
}

// v / a
inline Vector3 Vector3::operator/(const double a) const
{
    return Vector3(m_x / a, m_y / a, m_z / a);
}
// ||v||
inline double Vector3::len(void) const
{
    return (sqrt(m_x * m_x + m_y * m_y + m_z * m_z));
}

// ||v||^2
inline double Vector3::len_squared(void) const
{
    return (m_x * m_x + m_y * m_y + m_z * m_z);
}

// v * v2 -- dot product
inline double Vector3::operator*(const Vector3 &v2) const
{
    return (m_x * v2.m_x + m_y * v2.m_y + m_z * v2.m_z);
}

// v ^ v2 -- cross product
inline Vector3 Vector3::operator^(const Vector3 &v2) const
{
    return (Vector3(m_y * v2.m_z - m_z * v2.m_y,
                    m_z * v2.m_x - m_x * v2.m_z,
                    m_x * v2.m_y - m_y * v2.m_x));
}

// += operator
inline Vector3 &Vector3::operator+=(const Vector3 &rhs)
{
    m_x += rhs.m_x;
    m_y += rhs.m_y;
    m_z += rhs.m_z;
    return (*this);
}

inline void Vector3::prettyPrint(void) const
{
    fprintf(stdout, "[ %f , %f , %f ]\n", m_x, m_y, m_z);
}

#endif