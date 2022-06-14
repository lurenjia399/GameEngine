#include "GeometryDescData.h"

FGeometryDescData::FGeometryDescData()
	: MeshComponet(nullptr)
	, IndexSize(0)
	, VertexSize(0)
	, IndexoffsetPosition(0)
	, VertexoffsetPostion(0)
	, IndexTypeSize(sizeof(uint16_t))
	, VertexTypeSize(sizeof(FVertex))
	, WorldMatrix(EngineMath::IdentityMatrix4x4())
	, TextureTransform(EngineMath::IdentityMatrix4x4())
{
}

UINT FGeometryDescData::GetVertexSizeInBytes() const
{
	return VertexSize * VertexTypeSize;
}

UINT FGeometryDescData::GetIndexSizeInBytes() const
{
	return IndexSize * IndexTypeSize;
}
