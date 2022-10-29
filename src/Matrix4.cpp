#include "Matrix4.h"
#include <cstdio>

Matrix4::Matrix4()
{
    col1 = Vector4(1.0, 0.0, 0.0, 0.0);
    col2 = Vector4(0.0, 1.0, 0.0, 0.0);
    col3 = Vector4(0.0, 0.0, 1.0, 0.0);
    col4 = Vector4(0.0, 0.0, 0.0, 1.0);
}

Matrix4::~Matrix4()
{
}

// M*c
Matrix4 Matrix4::operator*(const double c)
{
    Matrix4 tmp;
    tmp.col1 = col1 * c;
    tmp.col2 = col2 * c;
    tmp.col3 = col3 * c;
    tmp.col4 = col4 * c;
    // for (int i = 0; i < 9; ++i)
    //     tmp.data[i] = data[i] * c;

    return tmp;
}
// c*M
Matrix4 operator*(const double c, Matrix4 &m)
{
    Matrix4 tmp;
    tmp.col1 = m.col1 * c;
    tmp.col2 = m.col2 * c;
    tmp.col3 = m.col3 * c;
    tmp.col4 = m.col4 * c;
    // for (int i = 0; i < 9; ++i)
    //     tmp.data[i] = (m.data[i]) * c;

    return tmp;
}

//  M * v
Vector4 Matrix4::operator*(const Vector4 &v)
{
    Matrix4 tmp;
    Vector4 result;

    tmp.col1 = col1 * v.m_x;
    tmp.col2 = col2 * v.m_y;
    tmp.col3 = col3 * v.m_z;
    tmp.col4 = col4 * v.m_w;
    result = tmp.col1 + tmp.col2 + tmp.col3 + tmp.col4;

    return result;
}

// M1 * M2
Matrix4 Matrix4::operator*(const Matrix4 &m2)
{
    Matrix4 tmp;
    tmp.col1 = (*this) * m2.col1; // M *v1
    tmp.col2 = (*this) * m2.col2; // M *v2
    tmp.col3 = (*this) * m2.col3; // M *v3
    tmp.col4 = (*this) * m2.col4; // M *v4

    return tmp;
}

double &Matrix4::operator()(const unsigned int row, const unsigned int col)
{
    Vector4 *target;
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

    case 3:
        target = &col4;
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
    case 3:
        return (*target).m_w;
        break;

    default:
        fprintf(stderr, "Wrong row index: %ud .\n", row);
        break;
    }
}

Matrix4 Matrix4::identity()
{
    Matrix4 tmp;
    return tmp;
}

Matrix4 Matrix4::transpose()
{
    Matrix4 tmp;
    tmp.col1.m_x = col1.m_x;
    tmp.col1.m_y = col2.m_x;
    tmp.col1.m_z = col3.m_x;

    tmp.col2.m_x = col1.m_y;
    tmp.col2.m_y = col2.m_y;
    tmp.col2.m_z = col3.m_y;

    tmp.col3.m_x = col1.m_z;
    tmp.col3.m_y = col2.m_z;
    tmp.col3.m_z = col3.m_z;

    tmp.col4.m_x = col1.m_w;
    tmp.col4.m_y = col2.m_w;
    tmp.col4.m_z = col3.m_w;

    return tmp;
}

void Matrix4::prettyPrint()
{
    fprintf(stdout, "\t| %f , %f , %f , %f |\n", col1.m_x, col2.m_x, col3.m_x, col4.m_x);
    fprintf(stdout, "\t| %f , %f , %f , %f |\n", col1.m_y, col2.m_y, col3.m_y, col4.m_y);
    fprintf(stdout, "\t| %f , %f , %f , %f |\n", col1.m_z, col2.m_z, col3.m_z, col4.m_z);
    fprintf(stdout, "\t| %f , %f , %f , %f |\n\n", col1.m_w, col2.m_w, col3.m_w, col4.m_w);
}