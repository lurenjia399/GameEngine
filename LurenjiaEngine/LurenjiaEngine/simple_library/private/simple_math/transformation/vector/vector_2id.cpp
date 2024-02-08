#include "simple_library/public/simple_math/transformation/vector/vector_2id.h"
#include <math.h>
fvector_2id::fvector_2id()
	:x(0)
	,y(0)
{
}

fvector_2id::fvector_2id(int in_x, int in_y)
	: x(in_x)
	, y(in_y)
{

}

fvector_2id::fvector_2id(float in_x, float in_y)
	: x(in_x)
	, y(in_y)
{

}

fvector_2id::fvector_2id(int in_value)
	:x(in_value)
	, y(in_value)
{
}

float fvector_2id::len()
{
	return sqrt(x * x + y * y);
}

void fvector_2id::normalize()
{
	*this /= len();
}