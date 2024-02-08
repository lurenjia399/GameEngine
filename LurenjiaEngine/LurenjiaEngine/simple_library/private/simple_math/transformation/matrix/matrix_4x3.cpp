#include "simple_library/public/simple_math/transformation/matrix/matrix_4x3.h"
#include "simple_library/public/simple_math/transformation/matrix/matrix_4x4.h"
#include <math.h>
#include <assert.h>

fmatrix_4x3::fmatrix_4x3()
{
	identity();
}

fmatrix_4x3::fmatrix_4x3(const fmatrix_4x4& in_matrix)
	: m11(in_matrix.m11), m12(in_matrix.m12), m13(in_matrix.m13)
	, m21(in_matrix.m21), m22(in_matrix.m22), m23(in_matrix.m23)
	, m31(in_matrix.m31), m32(in_matrix.m32), m33(in_matrix.m33)
	, x(0.f), y(0.f), z(0.f)
{

}

fmatrix_4x3::fmatrix_4x3(
	float in_m11, float in_m12, float in_m13, 
	float in_m21, float in_m22, float in_m23, 
	float in_m31, float in_m32, float in_m33, 
	float in_x,   float in_y,   float in_z)
	: m11(in_m11), m12(in_m12), m13(in_m13)
	, m21(in_m21), m22(in_m22), m23(in_m23)
	, m31(in_m31), m32(in_m32), m33(in_m33)
	, x(in_x), y(in_y), z(in_z)
{
}

void fmatrix_4x3::identity()
{
	*this = fmatrix_4x3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		0.f, 0.f, 0.f);
}

fmatrix_4x3 fmatrix_4x3::inversion(const fmatrix_4x3& in_m)
{
	//行列式
	float det = determinant(in_m);

	//判定是否有奇异矩阵 行列式如果为0 就代表没有逆矩阵
	assert(fabs(det) > 0.00001);

	float det_tmp = 1.f / det;

	//计算3x3 伴随乘以行列式
	fmatrix_4x3 m;
	m.m11 = (in_m.m22 * in_m.m33 - in_m.m23 * in_m.m32) * det_tmp;
	m.m12 = (in_m.m13 * in_m.m32 - in_m.m12 * in_m.m23) * det_tmp;
	m.m13 = (in_m.m12 * in_m.m23 - in_m.m13 * in_m.m22) * det_tmp;

	m.m21 = (in_m.m23 * in_m.m31 - in_m.m21 * in_m.m33) * det_tmp;
	m.m22 = (in_m.m11 * in_m.m33 - in_m.m13 * in_m.m31) * det_tmp;
	m.m23 = (in_m.m13 * in_m.m21 - in_m.m11 * in_m.m23) * det_tmp;

	m.m31 = (in_m.m21 * in_m.m32 - in_m.m22 * in_m.m31) * det_tmp;
	m.m32 = (in_m.m12 * in_m.m31 - in_m.m11 * in_m.m32) * det_tmp;
	m.m33 = (in_m.m11 * in_m.m22 - in_m.m12 * in_m.m21) * det_tmp;

	//平移的逆
	m.x = -(in_m.x * in_m.m11 + in_m.y * in_m.m21 + in_m.z * in_m.m31);
	m.y = -(in_m.x * in_m.m12 + in_m.y * in_m.m22 + in_m.z * in_m.m32);
	m.z = -(in_m.x * in_m.m13 + in_m.y * in_m.m23 + in_m.z * in_m.m33);

	return m;
}

void fmatrix_4x3::inversion()
{
	*this = inversion(*this);
}

float fmatrix_4x3::determinant(const fmatrix_4x3 &in_m)
{
	return in_m.m11 * (in_m.m22 * in_m.m33-in_m.m23 * in_m.m32)
		+ in_m.m12 * (in_m.m23 * in_m.m31 - in_m.m21 * in_m.m33
		+ in_m.m13 * (in_m.m21 * in_m.m32 -in_m.m22 * in_m.m31));
}

float fmatrix_4x3::determinant()
{
	return determinant(*this);
}
