#include "simple_library/public/simple_math/transformation/matrix/matrix_4x4.h"
#include "simple_library/public/simple_math/transformation/matrix/matrix_3x3.h"
#include "simple_library/public/simple_math/transformation/matrix/matrix_4x3.h"
#include "simple_library/public/simple_math/transformation/vector/vector_3d.h"
#include "simple_library/public/simple_math/transformation/quaternion/quat.h"
#include <math.h>

fmatrix_4x4::fmatrix_4x4()
{
    *this = identity();
}

fmatrix_4x4::fmatrix_4x4(const fmatrix_4x3& in_matrix)
    : m11(in_matrix.m11), m12(in_matrix.m12), m13(in_matrix.m13), m14(0.f)
    , m21(in_matrix.m21), m22(in_matrix.m22), m23(in_matrix.m23), m24(0.f)
    , m31(in_matrix.m31), m32(in_matrix.m32), m33(in_matrix.m33), m34(0.f)
    , m41(in_matrix.x), m42(in_matrix.y), m43(in_matrix.z), m44(1.f)
{
}

fmatrix_4x4::fmatrix_4x4(const fmatrix_3x3& in_matrix)
    : m11(in_matrix.m11), m12(in_matrix.m12), m13(in_matrix.m13), m14(0.f)
    , m21(in_matrix.m21), m22(in_matrix.m22), m23(in_matrix.m23), m24(0.f)
    , m31(in_matrix.m31), m32(in_matrix.m32), m33(in_matrix.m33), m34(0.f)
    , m41(0.f),           m42(0.f),           m43(0.f),           m44(1.f)
{

}

fmatrix_4x4::fmatrix_4x4(
    float in_m11, float in_m12, float in_m13, float in_m14, 
    float in_m21, float in_m22, float in_m23, float in_m24, 
    float in_m31, float in_m32, float in_m33, float in_m34, 
    float in_m41, float in_m42, float in_m43, float in_m44)
    : m11(in_m11), m12(in_m12), m13(in_m13), m14(in_m14)
    , m21(in_m21), m22(in_m22), m23(in_m23), m24(in_m24)
    , m31(in_m31), m32(in_m32), m33(in_m33), m34(in_m34)
    , m41(in_m41), m42(in_m42), m43(in_m43), m44(in_m44)
{

}

fmatrix_4x4& fmatrix_4x4::operator=(const fmatrix_3x3& a)
{
    m11 = a.m11; m12 = a.m12; m13 = a.m13; //m14 = 0.f,
    m21 = a.m21; m22 = a.m22; m23 = a.m23; //m24 = 0.f,
    m31 = a.m31; m32 = a.m32; m33 = a.m33; //m34 = 0.f;
  //  m41 = 0.f;   m42 = 0.f;   m43 = 0.f;   m44 = 0.f;

    return *this;
}

void fmatrix_4x4::inertia_to_object(const frotator& in_rot)
{
    fmatrix_3x3 m;
    m.inertia_to_object(in_rot);

   *this = m;
}

void fmatrix_4x4::object_to_inertia(const frotator& in_rot)
{
    fmatrix_3x3 m;
    m.object_to_inertia(in_rot);

    *this = m;
}

float fmatrix_4x4::determinant()const
{
   //ÏÈ½µÎ¬
    //m11
    float m11_algebraic_cofactor = fmatrix_3x3(
        m22, m23, m24,
        m32, m33, m34,
        m42, m43, m44).Determinant();
   
    //m12
    float m12_algebraic_cofactor = fmatrix_3x3(
        m21, m23, m24,
        m31, m33, m34,
        m41, m43, m44).Determinant();
    
    //m13
    float m13_algebraic_cofactor = fmatrix_3x3(
        m21, m22, m24,
        m31, m32, m34,
        m41, m42, m44).Determinant();
    
    //m14
    float m14_algebraic_cofactor = fmatrix_3x3(
        m21, m22, m23,
        m31, m32, m33,
        m41, m42, m43).Determinant();

    return  m11 * m11_algebraic_cofactor -
            m12 * m12_algebraic_cofactor +
            m13 * m13_algebraic_cofactor -
            m14 * m13_algebraic_cofactor;
}

fmatrix_4x4 fmatrix_4x4::identity()
{
    return fmatrix_4x4(
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f);
}

void fmatrix_4x4::transpose()
 {
    *this = to_transpose();
}

fmatrix_4x4 fmatrix_4x4::to_transpose()
{
    fmatrix_4x4 matrix_4x4 = *this;

    //½»»»º¯Êý
    auto swap_float = [](float& a, float& b)
    {
        float tmp = a;
        a = b;
        b = tmp;
    };

    swap_float(matrix_4x4.m21, matrix_4x4.m12);
    swap_float(matrix_4x4.m31, matrix_4x4.m13);
    swap_float(matrix_4x4.m41, matrix_4x4.m14);
    swap_float(matrix_4x4.m32, matrix_4x4.m23);
    swap_float(matrix_4x4.m42, matrix_4x4.m24);
    swap_float(matrix_4x4.m43, matrix_4x4.m34);

    return matrix_4x4;
}

fvector_3d fmatrix_4x4::get_translation()
{
    return fvector_3d(m41, m42, m43);
}

fquat fmatrix_4x4::get_quat() const
{
    float x, y, z, w;
    float float_tmp[4];

    int index_flag = 0;

    float_tmp[0] = m11 + m22 + m33;
    float_tmp[1] = m11 - m22 - m33;
    float_tmp[2] = m22 - m11 - m33;
    float_tmp[3] = m33 - m11 - m22;

    float float_big = float_tmp[0];
    for (int i = 1; i < 4; i++)
    {
        if (float_big < float_tmp[i])
        {
            float_big = float_tmp[i];
            index_flag = i;
        }
    }

    float_big = sqrtf(float_big + 1.0f) * 0.5f;
    float float_mult = 0.25f / float_big;
    switch (index_flag)
    {
    case 0:
        w = float_big;
        x = (m23 - m32) * float_mult;
        y = (m31 - m32) * float_mult;
        z = (m12 - m21) * float_mult;
        break;
    case 1:
        x = float_big;
        w = (m23 - m32) * float_mult;
        y = (m12 + m21) * float_mult;
        z = (m31 + m13) * float_mult;
        break;
    case 2:
        y = float_big;
        w = (m31 - m13) * float_mult;
        x = (m12 + m21) * float_mult;
        z = (m23 + m32) * float_mult;
        break;
    case 3:
        z = float_big;
        w = (m12 - m21) * float_mult;
        x = (m31 + m13) * float_mult;
        y = (m23 + m32) * float_mult;
        break;
    }
    return fquat(x, y, z, w);
}
