// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "assert.h" 
#include "simple_library/public/simple_library_macro.h"

struct SIMPLE_LIBRARY_API fvector_color
{
	float r;
	float g;
	float b;
	float a;

public:
	fvector_color();

	fvector_color(float in_value);

	fvector_color(float in_r, float in_g, float in_b = 0, float in_a = 255);
};