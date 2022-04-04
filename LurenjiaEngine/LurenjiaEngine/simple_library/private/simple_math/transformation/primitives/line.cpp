#include "../../../../public/simple_math/transformation/primitives/line.h"
#include "../../../../public/simple_math/math_utils.h"

namespace primitives
{
	fline::fline()
		: k(1.f)
		, b(0.f)
	{

	}

	fline::fline(
		const fvector_2d& in_start_point,
		const fvector_2d& In_end_point,
		const fvector_4d& in_color)
		:start_point(in_start_point)
		,end_point(In_end_point)
		,color(in_color)
		,k(1.f)
		,b(0.f)
	{
		init();
	}

	void fline::init()
	{
		k = (end_point.y - start_point.y) / (end_point.x - start_point.x);
		b = start_point.y - k * start_point.x;
	}
	
	//y = kx + b
	bool is_inside_line(const fline &in_line, int x, int y)
	{
		float y2 = in_line.end_point.y;
		float y1 = in_line.start_point.y;

		int new_y = x * in_line.k + in_line.b;
		if (math_utils::scalar_neare_qual_int(new_y, y,2))
		{
			if (y2 < y1)
			{
				float tmp = y1;
				y1 = y2;
				y2 = tmp;
			}

			if (new_y >= y1 && new_y <= y2)
			{
				return true;
			}
		}

		return false;
	}
	bool onsegment(fvector_2d p1, fvector_2d p2, fvector_2d Q)
	{
		fvector_2d a = Q - p1;
		fvector_2d b = Q - p2;
		a.normalize();
		b.normalize();

		float y2 = p1.y;
		float y1 = p2.y;
		float x2 = p1.x;
		float x1 = p2.x;
		if (math_utils::scalar_neare_qual_float(fvector_2d::dot(a, b),-1.f,0.003f))
		{
			if (y2 < y1)
			{
				float tmp = y1;
				y1 = y2;
				y2 = tmp;
			}
			if (x2 < x1)
			{
				float tmp = x1;
				x1 = x2;
				x2 = tmp;
			}
			if (Q.y >= y1 && Q.y <= y2 &&
				Q.x >= x1 && Q.x <= x2)
			{
				return true;
			}
		}

		return false;
	}
}