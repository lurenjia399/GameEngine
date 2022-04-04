
#pragma once
#include "assert.h" 

struct fvector_color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

public:
	fvector_color();

	fvector_color(unsigned char in_value);

	fvector_color(unsigned char in_r, unsigned char in_g, unsigned char in_b = 0, unsigned char in_a = 255);
};