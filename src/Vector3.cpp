#include "Vector3.h"
#include "Normal3.h"
// default copy constructor
Vector3::Vector3(void) : m_x(0.0), m_y(0.0), m_z(0.0)
{
}
// constructor
Vector3::Vector3(const double x, const double y, const double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}
// constructor
Vector3::Vector3(const double a)
{
    m_x = a;
    m_y = a;
    m_z = a;
}
// copy constructor
Vector3::Vector3(const Vector3 &v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
}
// deconstructor
Vector3::~Vector3()
{
    return;
}

// assignment operator
Vector3 &Vector3::operator=(const Vector3 &rhs)
{
    if (this == &rhs)
        return (*this);
    else
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;

        return (*this);
    }
}

void Vector3::normalize(void)
{
    double len = this->len();
    m_x /= len;
    m_y /= len;
    m_z /= len;
    return;
}
Vector3 &Vector3::hat(void)
{

    double len = this->len();
    m_x /= len;
    m_y /= len;
    m_z /= len;
    return (*this);
}

// v + normal
Vector3 Vector3::operator+(const Normal3 &n) const
{
    return Vector3(m_x + n.m_x, m_y + n.m_y, m_z + n.m_z);
}
// v * n-- dot product
double Vector3::operator*(const Normal3 &n) const
{
    return (m_x * n.m_x + m_y * n.m_y + m_z * n.m_z);
}
