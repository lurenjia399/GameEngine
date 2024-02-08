// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "simple_library/public/simple_library_macro.h"

struct SIMPLE_LIBRARY_API fvector_2id
{
	int x;
	int y;

public:
	fvector_2id operator+(const fvector_2id& a)const
	{
		return fvector_2id(x + a.x, y + a.y);
	}

	fvector_2id operator-(const fvector_2id& a)const
	{
		return fvector_2id(x - a.x, y - a.y);
	}

	void operator-=(const fvector_2id& a)
	{
		*this = *this - a;
	}

	void operator+=(const fvector_2id& a)
	{
		*this = *this + a;
	}

	//向量是不能和标量相加 
	fvector_2id operator+(float k)const
	{
		return fvector_2id(x + k, y + k);
	}

	fvector_2id operator-(float k)const
	{
		return fvector_2id(x - k, y - k);
	}

	fvector_2id operator*(float k)const
	{
		return fvector_2id(x * k, y * k);
	}

	fvector_2id operator/(float k)const
	{
		return fvector_2id(x / k, y / k);
	}

	bool operator>(const fvector_2id& a)const
	{
		return x > a.x && y > a.y;
	}

	bool operator<(const fvector_2id& a)const
	{
		return x < a.x && y < a.y;
	}

	bool operator>=(const fvector_2id& a)const
	{
		return x >= a.x && y >= a.y;
	}

	bool operator<=(const fvector_2id& a)const
	{
		return x <= a.x&& y <= a.y;
	}

	fvector_2id operator/=(float k)
	{
		*this = *this / k;

		return *this;
	}

	fvector_2id operator*(const fvector_2id& a)const
	{
		return fvector_2id(x * a.x, y * a.y);
	}

	fvector_2id operator/(const fvector_2id& a)const
	{
		return fvector_2id(x / a.x, y / a.y);
	}

	fvector_2id();

	fvector_2id(int in_x, int in_y);
	fvector_2id(float in_x, float in_y);

	fvector_2id(int in_value);

	float len();

	void normalize();
};