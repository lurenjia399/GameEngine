#pragma once
#include "MeshType.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;
	static XMFLOAT4X4 IdentityMatrix4x4();
};