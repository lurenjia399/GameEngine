#pragma once
#include "../../LurenjiaEngine.h"
#include "../../Math/EngineMath.h"
struct FVertex
{
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor);

	XMFLOAT3 Location;
	XMFLOAT4 Color;
};

struct FMeshRenderingData
{
	vector<FVertex> VertexData;
	vector<uint16_t> IndexData;
};