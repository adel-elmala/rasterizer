#include "Matrix3.h"
#include <cstdio>

Matrix3::Matrix3()
{
    col1 = Vector3(1.0, 0.0, 0.0);
    col2 = Vector3(0.0, 1.0, 0.0);
    col3 = Vector3(0.0, 0.0, 1.0);
}

Matrix3::~Matrix3()
{
}

// M*c
Matrix3 Matrix3::operator*(const double c)
{
    Matrix3 tmp;
    tmp.col1 = col1 * c;
    tmp.col2 = col2 * c;
    tmp.col3 = col3 * c;
    // for (int i = 0; i < 9; ++i)
    //     tmp.data[i] = data[i] * c;

    return tmp;
}
// c*M
Matrix3 operator*(const double c, Matrix3 &m)
{
    Matrix3 tmp;
    tmp.col1 = m.col1 * c;
    tmp.col2 = m.col2 * c;
    tmp.col3 = m.col3 * c;
    // for (int i = 0; i < 9; ++i)
    //     tmp.data[i] = (m.data[i]) * c;

    return tmp;
}

//  M * v
Vector3 Matrix3::operator*(const Vector3 &v)
{
    Matrix3 tmp;
    Vector3 result;

    tmp.col1 = col1 * v.m_x;
    tmp.col2 = col2 * v.m_y;
    tmp.col3 = col3 * v.m_z;
    result = tmp.col1 + tmp.col2 + tmp.col3;

    return result;
}

// M1 * M2
Matrix3 Matrix3::operator*(const Matrix3 &m2)
{
    Matrix3 tmp;
    tmp.col1 = (*this) * m2.col1; // M *v1
    tmp.col2 = (*this) * m2.col2; // M *v2
    tmp.col3 = (*this) * m2.col3; // M *v3

    return tmp;
}

double &Matrix3::operator()(const unsigned int row, const unsigned int col)
{
    Vector3 *target;
    switch (col)
    {

    case 0:
        target = &col1;
        break;

    case 1:
        target = &col2;
        break;

    case 2:
        target = &col3;
        break;

    default:
        fprintf(stderr, "Wrong column index: %ud .\n", col);
        break;
    }

    switch (row)
    {

    case 0:
        return (*target).m_x;
        break;
    case 1:
        return (*target).m_y;
        break;
    case 2:
        return (*target).m_z;
        break;

    default:
        fprintf(stderr, "Wrong row index: %ud .\n", row);
        break;
    }
}

Matrix3 Matrix3::identity()
{
    Matrix3 tmp;
    return tmp;
}

Matrix3 Matrix3::transpose()
{
    Matrix3 tmp;
    tmp.col1.m_x = col1.m_x;
    tmp.col1.m_y = col2.m_x;
    tmp.col1.m_z = col3.m_x;

    tmp.col2.m_x = col1.m_y;
    tmp.col2.m_y = col2.m_y;
    tmp.col2.m_z = col3.m_y;

    tmp.col3.m_x = col1.m_z;
    tmp.col3.m_y = col2.m_z;
    tmp.col3.m_z = col3.m_z;

    return tmp;
}

void Matrix3::prettyPrint()
{
    fprintf(stdout, "\t| %f , %f , %f |\n", col1.m_x, col2.m_x, col3.m_x);
    fprintf(stdout, "\t| %f , %f , %f |\n", col1.m_y, col2.m_y, col3.m_y);
    fprintf(stdout, "\t| %f , %f , %f |\n\n", col1.m_z, col2.m_z, col3.m_z);
}