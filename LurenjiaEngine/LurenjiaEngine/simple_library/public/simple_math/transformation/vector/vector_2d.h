// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "simple_library/public/simple_library_macro.h"

struct SIMPLE_LIBRARY_API fvector_2d
{
	float x;
	float y;
public:
	fvector_2d();

	fvector_2d(float in_x, float in_y);

	fvector_2d(float in_value);

	float len();

	static float dot(const fvector_2d& a, const fvector_2d& b);

	static float cross_product(const fvector_2d& a, const fvector_2d& b);

	char *to_string(char *buff)const;

	void normalize();
public:
	fvector_2d operator+(const fvector_2d& a)const
	{
		return fvector_2d(x + a.x , y + a.y);
	}

	fvector_2d operator-(const fvector_2d& a)const
	{
		return fvector_2d(x- a.x, y - a.y);
	}

	bool operator>(const fvector_2d& a)const
	{
		return x > a.x && y > a.y;
	}

	bool operator<(const fvector_2d& a)const
	{
		return x < a.x && y < a.y;
	}

	bool operator>=(const fvector_2d& a)const
	{
		return x >= a.x && y >= a.y;
	}

	bool operator<=(const fvector_2d& a)const
	{
		return x <= a.x&& y <= a.y;
	}

	void operator-=(const fvector_2d& a)
	{
		*this = *this - a;
	}

	void operator+=(const fvector_2d& a)
	{
		*this = *this + a;
	}

	//向量是不能和标量相加 
	fvector_2d operator+(float k)const
	{
		return fvector_2d(x + k, y + k);
	}

	fvector_2d operator-(float k)const
	{
		return fvector_2d(x - k, y - k);
	}

	fvector_2d operator*(float k)const
	{
		return fvector_2d(x * k, y * k);
	}

	fvector_2d operator/(float k)const
	{
		return fvector_2d(x / k, y / k);
	}

	fvector_2d operator/=(float k)
	{
		*this = *this / k;

		return *this;
	}

	fvector_2d operator*(const fvector_2d& a)const
	{
		return fvector_2d(x * a.x, y * a.y);
	}

	fvector_2d operator/(const fvector_2d& a)const
	{
		return fvector_2d(x / a.x, y / a.y);
	}


};