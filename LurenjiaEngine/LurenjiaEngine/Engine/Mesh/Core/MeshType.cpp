#include "MeshType.h"

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal, const XMFLOAT3& InUTangent, const XMFLOAT2& InTexCoord)
	: Pos(InPos)
	, Color(InColor)
	, Normal(InNormal)
	, UTangent(InUTangent)
	, TexCoord(InTexCoord)
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
