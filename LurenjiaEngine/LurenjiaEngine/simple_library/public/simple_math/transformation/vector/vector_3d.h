// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "assert.h" 
#include "simple_library/public/simple_library_macro.h"

struct fvector_4d;
struct SIMPLE_LIBRARY_API fvector_3d
{
	float x;
	float y;
	float z;

public:
	fvector_3d();
	fvector_3d(float in_value);
	fvector_3d(int in_a, int in_b, int in_c);
	fvector_3d(float in_a, float in_b, float in_c);

	char* to_string(char* buff);

public:

	fvector_3d& operator=(const fvector_4d& a);

	bool operator>(const fvector_3d& a)const
	{
		return x > a.x && y > a.y && z > a.z;
	}

	bool operator<(const fvector_3d& a)const
	{
		return x < a.x&& y < a.y && z < a.z;
	}

	bool operator>=(const fvector_3d& a)const
	{
		return x >= a.x && y >= a.y && z >= a.z;
	}

	bool operator<=(const fvector_3d& a)const
	{
		return x <= a.x&& y <= a.y&& z <= a.z;
	}

	bool operator!=(const fvector_3d& a)const
	{
		return x != a.x && y != a.y && z != a.z;
	}

	bool operator==(const fvector_3d& a)const
	{
		return x == a.x && y == a.y && z == a.z;
	}

	fvector_3d operator+(const fvector_3d& a)const
	{
		return fvector_3d(a.x + x, a.y + y,a.z + z);
	}

	fvector_3d operator-(const fvector_3d& a)const
	{
		return fvector_3d(x - a.x,y- a.y,z- a.z);
	}

	void operator-=(const fvector_3d& a)
	{
		*this = *this - a;
	}

	void operator+=(const fvector_3d& a)
	{
		*this = *this + a;
	}

	//向量不能这么做
	void operator+=(const float& a)
	{
		x += a;
		y += a;
		z += a;
	}

	//向量不能这么做
	void operator-=(const float& a)
	{
		x -= a;
		y -= a;
		z -= a;
	}

	fvector_3d operator+(const float& a)const
	{
		return fvector_3d(a + x, a + y, a + z);
	}

	fvector_3d operator-(const float& a)const
	{
		return fvector_3d(x - a, y - a, z - a);
	}

	fvector_3d operator*(const float& a)const
	{
		return fvector_3d(a * x, a * y, a * z);
	}

	fvector_3d operator/(const float& a)const
	{
		assert(a != 0.f);

		return fvector_3d(x / a, y / a, z / a);
	}

	fvector_3d operator*(const fvector_3d& a)const
	{
		return fvector_3d(a.x * x, a.y * y, a.z * z);
	}

	fvector_3d operator/(const fvector_3d& a)const
	{
		assert(x != 0.f);
		assert(y != 0.f);
		assert(z != 0.f);

		return fvector_3d(a.x / x, a.y / y, a.z / z);
	}

	fvector_3d operator*=(const fvector_3d& a)
	{
		*this = *this * a;

		return *this;
	}

	fvector_3d operator/=(const fvector_3d& a)
	{
		*this = *this / a;

		return *this;
	}

	fvector_3d operator/=(const float& a)
	{
		*this = *this / a;

		return *this;
	}

	fvector_3d operator^(const fvector_3d& a)const
	{
		return fvector_3d(a.y * z - a.z * y,x*a.z - a.x*z,a.x*y - a.y * x);
	}

	static float dot(const fvector_3d& a, const fvector_3d& b);

	void normalize();

	float len();

	static fvector_3d cross_product(const fvector_3d& a, const fvector_3d& b);
};