#include "MeshType.h"

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor)
	: Pos(InPos)
	, Color(InColor)
	, Normal(XMFLOAT3(1, 1, 1))
{
}
 