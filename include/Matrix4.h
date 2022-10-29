#ifndef MATRIX4_H
#define MATRIX4_H

// #include "Vector3.h"
#include "Vector4.h"

class Matrix4
{
public:
    Vector4 col1;
    Vector4 col2;
    Vector4 col3;
    Vector4 col4;

    Matrix4();
    ~Matrix4();

    Matrix4 operator*(const double c);
    Vector4 operator*(const Vector4 &v);
    Matrix4 operator*(const Matrix4 &m2);
    double &operator()(const unsigned int row, const unsigned int col);
    Matrix4 identity();
    Matrix4 transpose();
    void prettyPrint();
};

Matrix4 operator*(const double c, Matrix4 &m);

#endif