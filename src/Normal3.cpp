#include "Normal3.h"
// default copy constructor
Normal3::Normal3(void) : m_x(0.0), m_y(0.0), m_z(0.0)
{
}
// constructor
Normal3::Normal3(const double x,const double y,const double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}
// constructor
Normal3::Normal3(const double a)
{
    m_x = a;
    m_y = a;
    m_z = a;
}
// copy constructor
Normal3::Normal3(const Normal3 &v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
}
// deconstructor
Normal3::~Normal3()
{
    return;
}

// assignment operator
Normal3 &Normal3::operator=(const Normal3 &rhs)
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
