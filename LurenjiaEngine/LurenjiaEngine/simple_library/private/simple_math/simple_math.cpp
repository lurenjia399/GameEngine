#include "simple_library/public/simple_math/simple_math.h"

unsigned int get_uint32_random(unsigned int in_max)
{
	if (in_max > 0)
	{
		return (unsigned int)(get_float_random_static() * (in_max - 0) + 0);
	}

	return 0;
}
