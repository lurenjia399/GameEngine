#pragma once
#include "MeshType.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;
	XMFLOAT4X4 TextureTransformation;

	UINT MaterialIndex;
	int xx1;
	int xx2;
	int xx3;
};	
	
	