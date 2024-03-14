#include "../../../../public/simple_math/transformation/vector/vector_color.h"

fvector_color::fvector_color()
	:r(0.f)
	,g(0.f)
	,b(0.f)
	,a(1.f)
{

}

fvector_color::fvector_color(float in_value)
	:r(in_value)
	,g(in_value)
	,b(in_value)
	,a(255)
{

}

fvector_color::fvector_color(float in_r, float in_g, float in_b, float in_a)
	:r(in_r)
	,g(in_g)
	,b(in_b)
	,a(in_a)
{
}
