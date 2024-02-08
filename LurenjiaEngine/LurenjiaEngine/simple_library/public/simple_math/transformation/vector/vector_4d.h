// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "assert.h" 
#include "simple_library/public/simple_library_macro.h"

struct fvector_3d;
struct SIMPLE_LIBRARY_API fvector_4d
{
	float x;
	float y;
	float z;
	float w;

	fvector_4d()
	:x(0.f)
	,y(0.f)
	,z(0.f)
	,w(1.f)
	{

	}

	fvector_4d(float in_value);
	fvector_4d(const fvector_3d &in_v);
	fvector_4d(float a, float b, float c, float d);

public:
	bool operator>(const fvector_4d& a)const
	{
		return x > a.x && y > a.y && z > a.z && w > a.w;
	}

	bool operator<(const fvector_4d& a)const
	{
		return x < a.x&& y < a.y&& z < a.z && w < a.w;
	}

	bool operator>=(const fvector_4d& a)const
	{
		return x >= a.x && y >= a.y && z >= a.z && w >= a.w;
	}

	bool operator<=(const fvector_4d& a)const
	{
		return x <= a.x&& y <= a.y&& z <= a.z&& w <= a.w;
	}

	fvector_4d operator+(const fvector_4d& a) const
	{
		return fvector_4d(a.x + x, a.y + y, a.z + z, a.w + w);
	}

	fvector_4d operator-(const fvector_4d& a) const
	{
		return fvector_4d(x - a.x, y - a.y, z - a.z, a.w + w);
	}

	fvector_4d operator*(const fvector_4d& a)const
	{
		return fvector_4d(a.x * x, a.y * y, a.z * z, a.w * w);
	}

	fvector_4d operator/(const fvector_4d& a)const
	{
		assert(x != 0.f);
		assert(y != 0.f);
		assert(z != 0.f);

		return fvector_4d(a.x / x, a.y / y, a.z / z, a.w / w);
	}

	fvector_4d operator*(const float& a)const
	{
		return fvector_4d(a * x, a * y, a * z, a * w);
	}

	fvector_4d operator/(const float& a)const
	{
		assert(a != 0.f);

		return fvector_4d(x / a, y / a, z / a, w / a);
	}

	fvector_4d &operator*=(const fvector_4d& a)
	{
		*this = *this * a;

		return *this;
	}

	fvector_4d& operator*(const float& a)
	{
		*this = *this * a;

		return *this;
	}

	fvector_4d& operator/=(const fvector_4d& a)
	{
		*this = *this / a;

		return *this;
	}

	fvector_4d& operator/=(const float& a)
	{
		*this = *this / a;

		return *this;
	}

	void normalize();

	static fvector_4d cross_product(const fvector_4d& a, const fvector_4d& b);

	static float dot(const fvector_4d& a, const fvector_4d& b);

	char *to_string(char* buff) const;
};