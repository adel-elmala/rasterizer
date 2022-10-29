#ifndef Normal3_H
#define Normal3_H

#include <cstdio>
#include <cmath>

#include "Vector3.h"
class Normal3
{

public:
    double m_x;
    double m_y;
    double m_z;

    // default constructor
    Normal3(void);
    // constructor
    Normal3(const double a);
    // constructor
    Normal3(const double x, const double y, const double z);
    // copy constructor
    Normal3(const Normal3 &v);
    // deconstructor
    ~Normal3();
    Normal3 operator-() const;                  // -n
    Normal3 operator+(const Normal3 &m) const; // n + m
    double operator*(const Vector3 &v) const;  // n * v -- dot product
    Normal3 operator*(const double a) const;    // n * a
    Vector3 operator+(const Vector3 &v) const; // n + v
    Normal3 &operator=(const Normal3 &rhs);     // assignment operator
    Normal3 &operator+=(const Normal3 &rhs);    // += operator
    
    void prettyPrint(void) const;
};
// -n
inline Normal3 Normal3::operator-() const
{
    return (Normal3(-m_x, -m_y, -m_z));
}

// n + m
inline Normal3 Normal3::operator+(const Normal3 &m) const
{
    return Normal3(m_x + m.m_x, m_y + m.m_y, m_z + m.m_z);
}

// n * v -- dot product
inline double Normal3::operator*(const Vector3 &v) const
{
    return (m_x * v.m_x + m_y * v.m_y + m_z * v.m_z);
}

// n * a
inline Normal3 Normal3::operator*(const double a) const
{
    return Normal3(m_x * a, m_y * a, m_z * a);
}
// a * n
Normal3 operator*(const double a, const Normal3 &v);
inline Normal3 operator*(const double a, const Normal3 &v)
{
    return Normal3(v.m_x * a, v.m_y * a, v.m_z * a);
}

// n + v
inline Vector3 Normal3::operator+(const Vector3 &v) const
{
    return Vector3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
}


// += operator
inline Normal3 &Normal3::operator+=(const Normal3 &rhs)
{
    m_x += rhs.m_x;
    m_y += rhs.m_y;
    m_z += rhs.m_z;
    return (*this);
}

inline void Normal3::prettyPrint(void) const
{
    fprintf(stdout, "[ %f , %f , %f ]\n", m_x, m_y, m_z);
}

#endif