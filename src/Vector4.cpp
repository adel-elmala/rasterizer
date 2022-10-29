#include "Vector4.h"
#include "Normal3.h"
// default copy constructor
Vector4::Vector4(void) : m_x(0.0), m_y(0.0), m_z(0.0),m_w(0)
{
}
// constructor
Vector4::Vector4(const double x, const double y, const double z,const double w)
{
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
}
// constructor
Vector4::Vector4(const double a)
{
    m_x = a;
    m_y = a;
    m_z = a;
    m_w = a;
}
// copy constructor
Vector4::Vector4(const Vector3 &v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
    m_w = 0;
}
// deconstructor
Vector3::~Vector3()
{
    return;
}

// assignment operator
Vector4 &Vector4::operator=(const Vector4 &rhs)
{
    if (this == &rhs)
        return (*this);
    else
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        m_w = rhs.m_w;

        return (*this);
    }
}

void Vector4::normalize(void)
{
    double len = this->len();
    m_x /= len;
    m_y /= len;
    m_z /= len;
    m_w /= len;

    return;
}
Vector4 &Vector4::hat(void)
{

    double len = this->len();
    m_x /= len;
    m_y /= len;
    m_z /= len;
    m_w /= len;
    return (*this);
}

// // v + normal
// Vector4 Vector4::operator+(const Normal3 &n) const
// {
//     return Vector4(m_x + n.m_x, m_y + n.m_y, m_z + n.m_z);
// }
// v * n-- dot product
// double Vector4::operator*(const Normal3 &n) const
// {
//     return (m_x * n.m_x + m_y * n.m_y + m_z * n.m_z);
// }
