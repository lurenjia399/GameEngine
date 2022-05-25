#include "MeshType.h"

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal)
	: Pos(InPos)
	, Color(InColor)
	, Normal(InNormal)
	, UTangent(0.f, 0.f, 0.f)
{
}

UINT FMeshRenderingData::GetVertexSizeInBytes() const
{
	return VertexData.size() * sizeof(FVertex);
}

UINT FMeshRenderingData::GetIndexSizeInBytes() const
{
	return IndexData.size() * sizeof(uint16_t);
}
