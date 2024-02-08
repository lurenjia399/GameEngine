#pragma once
#include "simple_library/public/simple_math/transformation/vector/vector_2d.h"
#include "simple_library/public/simple_math/transformation/vector/vector_4d.h"
#include "simple_library/public/simple_library_macro.h"

namespace primitives
{
	struct SIMPLE_LIBRARY_API ftriangle
	{
		fvector_2d point_1;
		fvector_2d point_2;
		fvector_2d point_3;

		fvector_4d Color;
	};

	//判定一个点是不是在三角形里面
	bool SIMPLE_LIBRARY_API is_inside_triangle(ftriangle in_triangle, int x, int y);
}