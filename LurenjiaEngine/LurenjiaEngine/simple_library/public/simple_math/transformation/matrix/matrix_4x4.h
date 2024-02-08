#pragma once
#include "simple_library/public/simple_library_macro.h"

struct fmatrix_3x3;
struct fmatrix_4x3;
struct frotator;
struct fvector_3d;
struct fquat;

struct SIMPLE_LIBRARY_API fmatrix_4x4
{
	float m11; float m12; float m13; float m14;
	float m21; float m22; float m23; float m24;
	float m31; float m32; float m33; float m34;
	float m41; float m42; float m43; float m44;

public:
	fmatrix_4x4();
	
	fmatrix_4x4(const fmatrix_4x3& in_matrix);

	fmatrix_4x4(const fmatrix_3x3& in_matrix);

	fmatrix_4x4(
		float in_m11, float in_m12, float in_m13, float in_m14,
		float in_m21, float in_m22, float in_m23, float in_m24,
		float in_m31, float in_m32, float in_m33, float in_m34,
		float in_m41, float in_m42, float in_m43, float in_m44);

	fmatrix_4x4& operator=(const fmatrix_3x3& a);

	void operator+=(const fmatrix_4x4& a)
	{
		*this = *this + a;
	}

	fmatrix_4x4 operator+(const fmatrix_4x4& a)const
	{
		return fmatrix_4x4(
			m11 + a.m11, m12 + a.m12, m13 + a.m13, m14 + a.m14,
			m21 + a.m21, m22 + a.m22, m23 + a.m23, m24 + a.m24,
			m31 + a.m31, m32 + a.m32, m33 + a.m33, m34 + a.m34,
			m41 + a.m41, m42 + a.m42, m43 + a.m43, m44 + a.m44);
	}

	void operator-=(const fmatrix_4x4& a)
	{
		*this = *this - a;
	}

	fmatrix_4x4 operator-(const fmatrix_4x4& a)const
	{
		return fmatrix_4x4(
			m11 - a.m11, m12 - a.m12, m13 - a.m13, m14 - a.m14,
			m21 - a.m21, m22 - a.m22, m23 - a.m23, m24 - a.m24,
			m31 - a.m31, m32 - a.m32, m33 - a.m33, m34 - a.m34,
			m41 - a.m41, m42 - a.m42, m43 - a.m43, m44 - a.m44);
	}

	fmatrix_4x4 operator*=(const fmatrix_4x4& a)
	{
		return *this * a;
	}

	fmatrix_4x4 operator*(const fmatrix_4x4& a)const
	{
		return fmatrix_4x4(
			m11 * a.m11 + m12 * a.m21 + m13 * a.m31 + m14 * a.m41,
			m11 * a.m12 + m12 * a.m22 + m13 * a.m32 + m14 * a.m42,
			m11 * a.m13 + m12 * a.m23 + m13 * a.m33 + m14 * a.m43,
			m11 * a.m14 + m12 * a.m24 + m13 * a.m34 + m14 * a.m44,
			
			m21* a.m11 + m22 * a.m21 + m23 * a.m31 + m24 * a.m41,
			m21* a.m12 + m22 * a.m22 + m23 * a.m32 + m24 * a.m42,
			m21* a.m13 + m22 * a.m23 + m23 * a.m33 + m24 * a.m43,
			m21* a.m14 + m22 * a.m24 + m23 * a.m34 + m24 * a.m44,
			
			m31* a.m11 + m32 * a.m21 + m33 * a.m31 + m34 * a.m41,
			m31* a.m12 + m32 * a.m22 + m33 * a.m32 + m34 * a.m42,
			m31* a.m13 + m32 * a.m23 + m33 * a.m33 + m34 * a.m43,
			m31* a.m14 + m32 * a.m24 + m33 * a.m34 + m34 * a.m44,
		
			m41* a.m11 + m42 * a.m21 + m43 * a.m31 + m44 * a.m41,
			m41* a.m12 + m42 * a.m22 + m43 * a.m32 + m44 * a.m42,
			m41* a.m13 + m42 * a.m23 + m43 * a.m33 + m44 * a.m43,
			m41* a.m14 + m42 * a.m24 + m43 * a.m34 + m44 * a.m44);
	}

public:

	//惯性->物体
	void inertia_to_object(const frotator& in_rot);
	//物体->惯性
	void object_to_inertia(const frotator& in_rot);

	//行列式
	float determinant()const;

	//单位化
	static fmatrix_4x4 identity();

	void transpose();

	fmatrix_4x4 to_transpose();

public:
	fvector_3d get_translation();

	fquat get_quat()const;
};