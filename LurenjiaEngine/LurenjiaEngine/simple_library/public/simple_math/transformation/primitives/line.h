#pragma once
#include "../vector/vector_2d.h"
#include "../vector/vector_4d.h"
#include "simple_library/public/simple_library_macro.h"

namespace primitives
{
	struct SIMPLE_LIBRARY_API fline
	{
		fline();

		fline(
			const fvector_2d &in_start_point,
			const fvector_2d &In_end_point,
			const fvector_4d &in_color = fvector_4d(255.f, 255.f, 255.f,1.f));

		void init();

		fvector_2d start_point;
		fvector_2d end_point;

		fvector_4d color;

		float k;
		float b;
	};

	//判定一个点是不是在线里面
	bool is_inside_line(const fline& in_line, int x, int y);


	bool onsegment(fvector_2d pi, fvector_2d pj, fvector_2d Q);

}