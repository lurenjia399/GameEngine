#include "MeshType.h"

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal, const XMFLOAT3& InUTangent, const XMFLOAT2& InTexCoord)
	: Pos(InPos)
	, Color(InColor)
	, Normal(InNormal)
	, UTangent(InUTangent)
	, TexCoord(InTexCoord)
{
}

UINT FVertexRenderingData::GetVertexSizeInBytes() const
{
	return static_cast<UINT>(VertexData.size()) * sizeof(FVertex);
}

UINT FVertexRenderingData::GetIndexSizeInBytes() const
{
	return static_cast<UINT>(IndexData.size()) * sizeof(uint16_t);
}
