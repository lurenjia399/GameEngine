// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "assert.h" 

struct fvector_4id
{
	int x;
	int y;
	int z;
	int w;

public:
	fvector_4id()
		:x(0)
		, y(0)
		, z(0)
		, w(1)
	{

	}

	fvector_4id(int in_value);

	fvector_4id(int a, int b, int c, int d);
};