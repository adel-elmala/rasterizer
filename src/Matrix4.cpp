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
    // tmp.col4 = col4 * c;
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
    double *val;
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
        val = &(*target).m_x;
        break;
    case 1:
        val = &(*target).m_y;
        break;
    case 2:
        val = &(*target).m_z;
        break;
    case 3:
        val = &(*target).m_w;
        break;

    default:
        fprintf(stderr, "Wrong row index: %ud .\n", row);
        break;
    }
    return *val;
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

Matrix4 Matrix4::inverse()
{
    double a11 = col1.m_x, a12 = col2.m_x, a13 = col3.m_x, a14 = col4.m_x;
    double a21 = col1.m_y, a22 = col2.m_y, a23 = col3.m_y, a24 = col4.m_y;
    double a31 = col1.m_z, a32 = col2.m_z, a33 = col3.m_z, a34 = col4.m_z;
    double a41 = col1.m_w, a42 = col2.m_w, a43 = col3.m_w, a44 = col4.m_w;

    double b0 = (a33 * a44 - a34 * a43);
    double b1 = (a32 * a44 - a34 * a42);
    double b2 = (a32 * a43 - a33 * a42);
    double b3 = (a31 * a44 - a34 * a41);
    double b4 = (a31 * a43 - a33 * a41);
    double b5 = (a31 * a42 - a32 * a41);
    double b6 = (a23 * a44 - a24 * a43);
    double b7 = (a22 * a44 - a24 * a42);
    double b8 = (a22 * a43 - a23 * a42);
    double b9 = (a21 * a44 - a24 * a41);
    double b10 = (a21 * a43 - a23 * a41);
    double b11 = (a21 * a42 - a22 * a41);
    double b12 = (a23 * a34 - a24 * a33);
    double b13 = (a22 * a34 - a24 * a32);
    double b14 = (a22 * a33 - a23 * a32);
    double b15 = (a21 * a34 - a24 * a31);
    double b16 = (a21 * a33 - a23 * a31);
    double b17 = (a21 * a32 - a22 * a31);

    double a11c = (a22 * b0) - (a23 * b1) + (a24 * b2);

    double a12c = -((a21 * b0) - (a23 * b3) + (a24 * b4));

    double a13c = (a21 * b1) - (a22 * b3) + (a24 * b5);

    double a14c = -((a21 * b2) - (a22 * b4) + (a23 * b5));

    double a21c = -((a12 * b0) - (a13 * b1) + (a14 * b2));

    double a22c = (a11 * b0) - (a13 * b3) + (a14 * b4);

    double a23c = -((a11 * b1) - (a12 * b3) + (a14 * b5));

    double a24c = (a21 * b2) - (a22 * b4) + (a23 * b5);

    double a31c = (a12 * b6) - (a13 * b7) + (a14 * b8);

    double a32c = -((a11 * b6) - (a13 * b9) + (a14 * b10));

    double a33c = (a11 * b7) - (a12 * b9) + (a14 * b11);

    double a34c = -((a11 * b8) - (a12 * b10) + (a13 * b11));

    double a41c = -((a12 * b12) - (a13 * b13) + (a14 * b14));

    double a42c = (a11 * b12) - (a13 * b15) + (a14 * b16);

    double a43c = -((a11 * b13) - (a12 * b15) + (a14 * b17));

    double a44c = (a11 * b14) - (a12 * b16) + (a13 * b17);

    double determ = a12 * a12c + a22 * a22c + a32 * a32c + a42 * a42c;

    Matrix4 inv;
    inv.col1.m_x = a11c;
    inv.col2.m_x = a21c;
    inv.col3.m_x = a31c;
    inv.col4.m_x = a41c;
    inv.col1.m_y = a12c;
    inv.col2.m_y = a22c;
    inv.col3.m_y = a32c;
    inv.col4.m_y = a42c;
    inv.col1.m_z = a13c;
    inv.col2.m_z = a23c;
    inv.col3.m_z = a33c;
    inv.col4.m_z = a43c;
    inv.col1.m_w = a14c;
    inv.col2.m_w = a24c;
    inv.col3.m_w = a34c;
    inv.col4.m_w = a44c;

    return inv * (1 / determ);
}

// returns the cofactor matrix
Matrix4 Matrix4::transformNormal()
{
    double a11 = col1.m_x, a12 = col2.m_x, a13 = col3.m_x, a14 = col4.m_x;
    double a21 = col1.m_y, a22 = col2.m_y, a23 = col3.m_y, a24 = col4.m_y;
    double a31 = col1.m_z, a32 = col2.m_z, a33 = col3.m_z, a34 = col4.m_z;
    double a41 = col1.m_w, a42 = col2.m_w, a43 = col3.m_w, a44 = col4.m_w;

    double b0 = (a33 * a44 - a34 * a43);
    double b1 = (a32 * a44 - a34 * a42);
    double b2 = (a32 * a43 - a33 * a42);
    double b3 = (a31 * a44 - a34 * a41);
    double b4 = (a31 * a43 - a33 * a41);
    double b5 = (a31 * a42 - a32 * a41);
    double b6 = (a23 * a44 - a24 * a43);
    double b7 = (a22 * a44 - a24 * a42);
    double b8 = (a22 * a43 - a23 * a42);
    double b9 = (a21 * a44 - a24 * a41);
    double b10 = (a21 * a43 - a23 * a41);
    double b11 = (a21 * a42 - a22 * a41);
    double b12 = (a23 * a34 - a24 * a33);
    double b13 = (a22 * a34 - a24 * a32);
    double b14 = (a22 * a33 - a23 * a32);
    double b15 = (a21 * a34 - a24 * a31);
    double b16 = (a21 * a33 - a23 * a31);
    double b17 = (a21 * a32 - a22 * a31);

    double a11c = (a22 * b0) - (a23 * b1) + (a24 * b2);

    double a12c = -((a21 * b0) - (a23 * b3) + (a24 * b4));

    double a13c = (a21 * b1) - (a22 * b3) + (a24 * b5);

    double a14c = -((a21 * b2) - (a22 * b4) + (a23 * b5));

    double a21c = -((a12 * b0) - (a13 * b1) + (a14 * b2));

    double a22c = (a11 * b0) - (a13 * b3) + (a14 * b4);

    double a23c = -((a11 * b1) - (a12 * b3) + (a14 * b5));

    double a24c = (a21 * b2) - (a22 * b4) + (a23 * b5);

    double a31c = (a12 * b6) - (a13 * b7) + (a14 * b8);

    double a32c = -((a11 * b6) - (a13 * b9) + (a14 * b10));

    double a33c = (a11 * b7) - (a12 * b9) + (a14 * b11);

    double a34c = -((a11 * b8) - (a12 * b10) + (a13 * b11));

    double a41c = -((a12 * b12) - (a13 * b13) + (a14 * b14));

    double a42c = (a11 * b12) - (a13 * b15) + (a14 * b16);

    double a43c = -((a11 * b13) - (a12 * b15) + (a14 * b17));

    double a44c = (a11 * b14) - (a12 * b16) + (a13 * b17);

    // double determ = a12 * a12c + a22 * a22c + a32 * a32c + a42 * a42c;

    Matrix4 inv;
    inv.col1.m_x = a11c;
    inv.col2.m_x = a12c;
    inv.col3.m_x = a13c;
    inv.col4.m_x = a14c;
    inv.col1.m_y = a21c;
    inv.col2.m_y = a22c;
    inv.col3.m_y = a23c;
    inv.col4.m_y = a24c;
    inv.col1.m_z = a31c;
    inv.col2.m_z = a32c;
    inv.col3.m_z = a33c;
    inv.col4.m_z = a34c;
    inv.col1.m_w = a41c;
    inv.col2.m_w = a42c;
    inv.col3.m_w = a43c;
    inv.col4.m_w = a44c;
    return inv;
}

void Matrix4::prettyPrint()
{
    fprintf(stdout, "\t| %f , %f , %f , %f |\n", col1.m_x, col2.m_x, col3.m_x, col4.m_x);
    fprintf(stdout, "\t| %f , %f , %f , %f |\n", col1.m_y, col2.m_y, col3.m_y, col4.m_y);
    fprintf(stdout, "\t| %f , %f , %f , %f |\n", col1.m_z, col2.m_z, col3.m_z, col4.m_z);
    fprintf(stdout, "\t| %f , %f , %f , %f |\n\n", col1.m_w, col2.m_w, col3.m_w, col4.m_w);
}