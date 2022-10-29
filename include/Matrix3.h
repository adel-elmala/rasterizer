#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector3.h"

class Matrix3
{
public:
    // double data[9] = {1.0, 0.0, 0.0,
    //                   0.0, 1.0, 0.0,
    //                   0.0, 0.0, 1.0}; // row-by-row
    Vector3 col1;
    Vector3 col2;
    Vector3 col3;

    Matrix3();
    ~Matrix3();

    Matrix3 operator*(const double c);
    Vector3 operator*(const Vector3 &v);
    Matrix3 operator*(const Matrix3 &m2);
    double &operator()(const unsigned int row,const unsigned int col);
    Matrix3 identity();
    Matrix3 transpose();
    void prettyPrint();
};

Matrix3 operator*(const double c, Matrix3 &m);

#endif