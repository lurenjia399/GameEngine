#pragma once
#include "simple_library/public/simple_library_macro.h"

struct frotator;
struct fmatrix_4x4;

struct SIMPLE_LIBRARY_API fmatrix_4x3
{
	float m11; float m12; float m13;
	float m21; float m22; float m23;
	float m31; float m32; float m33;
	float x;   float y;   float z;

public:
	fmatrix_4x3();
	fmatrix_4x3(const fmatrix_4x4 &in_m);
	fmatrix_4x3(
		float in_m11, float in_m12, float in_m13,
		float in_m21, float in_m22, float in_m23,
		float in_m31, float in_m32, float in_m33,
		float in_x, float in_y, float inz);

	void identity();
	fmatrix_4x3 inversion(const fmatrix_4x3& in_m);
	void inversion();
	float determinant(const fmatrix_4x3& in_m);
	float determinant();
};