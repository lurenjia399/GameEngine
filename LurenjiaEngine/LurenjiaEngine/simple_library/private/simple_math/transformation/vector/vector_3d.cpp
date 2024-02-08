#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "simple_library/public/simple_math/transformation/vector/vector_3d.h"
#include "simple_library/public/simple_math/transformation/vector/vector_4d.h"
#include <stdio.h>
#include <math.h>

fvector_3d::fvector_3d()
	:x(0.f)
	,y(0.f)
	,z(0.f)
{
}

fvector_3d::fvector_3d(float in_value)
	:x(in_value)
	,y(in_value)
	,z(in_value)
{
}

fvector_3d::fvector_3d(int in_a, int in_b, int in_c)
	:x(in_a)
	,y(in_b)
	,z(in_c)
{
}

fvector_3d::fvector_3d(float in_a, float in_b, float in_c)
	:x(in_a)
	,y(in_b)
	,z(in_c)
{
}

char* fvector_3d::to_string(char* buff)
{
	sprintf(buff, "(x=%f,y=%f,z=%f)", x, y,z);
	return buff;
}

fvector_3d& fvector_3d::operator=(const fvector_4d& a) 
{
	x = a.x;
	y = a.y;
	z = a.z;

	return *this;
}

float fvector_3d::dot(const fvector_3d& a, const fvector_3d& b)
{
	return a.x * b.x + a.y * b.y + b.z * a.z;
}

void fvector_3d::normalize()
{
	float value = sqrtf(x * x + y * y + z * z);

	*this /= value;
}

float fvector_3d::len()
{
	return sqrtf(x * x + y * y + z * z);
}

fvector_3d fvector_3d::cross_product(const fvector_3d& a, const fvector_3d& b)
{
	return fvector_3d(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}
