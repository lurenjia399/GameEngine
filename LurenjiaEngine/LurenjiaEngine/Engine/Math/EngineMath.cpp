#include "EngineMath.h"
#include "EngineMath.h"

namespace EngineMath
{
	XMFLOAT4X4 EngineMath::IdentityMatrix4x4()
	{
		return XMFLOAT4X4(
			1.0f, 0, 0, 0,
			0, 1.0f, 0, 0,
			0, 0, 1.0f, 0,
			0, 0, 0, 1.0f);
	}
}