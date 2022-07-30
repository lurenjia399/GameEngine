// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "assert.h" 

struct fvector_3id
{
	int x;
	int y;
	int z;

public:
	fvector_3id();
	fvector_3id(int in_value);
	fvector_3id(int in_a, int in_b, int in_c);
};