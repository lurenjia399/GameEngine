// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "assert.h" 
#include "simple_library/public/simple_library_macro.h"

struct SIMPLE_LIBRARY_API fvector_4id
{
	int x;
	int y;
	int z;
	int w;

public:
	fvector_4id()
		: x(0)
		, y(0)
		, z(0)
		, w(1)
	{

	}

	fvector_4id(int in_value);

	fvector_4id(int a, int b, int c, int d);

public:
	bool operator>(const fvector_4id& a)const
	{
		return x > a.x && y > a.y && z > a.z && w > a.w;
	}

	bool operator<(const fvector_4id& a)const
	{
		return x < a.x&& y < a.y&& z < a.z&& w < a.w;
	}

	bool operator>=(const fvector_4id& a)const
	{
		return x >= a.x && y >= a.y && z >= a.z && w >= a.w;
	}

	bool operator<=(const fvector_4id& a)const
	{
		return x <= a.x&& y <= a.y&& z <= a.z&& w <= a.w;
	}
};