#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include "../../../../public/simple_math/transformation/vector/vector_4d.h"
#include <math.h>
#include <stdio.h>

fvector_4d::fvector_4d(float in_value)
	: x(in_value)
	, y(in_value)
	, z(in_value)
	, w(in_value)
{

}

fvector_4d::fvector_4d(float a, float b, float c, float d)
	:x(a)
	, y(b)
	, z(c)
	, w(d)
{}

void fvector_4d::normalize()
{
	*this /= sqrtf(x * x + y * y + z * z);

	w = 0.f;
}

fvector_4d fvector_4d::cross_product(const fvector_4d& a, const fvector_4d& b)
{
	return fvector_4d(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
		0.0f);
}
char* fvector_4d::to_string(char* buff)const
{
	sprintf(buff, "(x=%f,y=%f,z=%f,w=%f)", x, y,z,w);
	return buff;
}

float fvector_4d::dot(const fvector_4d& a, const fvector_4d& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
