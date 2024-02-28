#include "GeometryDescData.h"

FGeometryDescData::FGeometryDescData()
	: MeshComponet(nullptr)
	, MeshHash(0)
	, IndexSize(0)
	, VertexSize(0)
	, IndexoffsetPosition(0)
	, VertexoffsetPostion(0)
	, IndexTypeSize(sizeof(uint16_t))
	, VertexTypeSize(sizeof(FVertex))
	, WorldMatrix(EngineMath::IdentityMatrix4x4())
	, TextureTransform(EngineMath::IdentityMatrix4x4())
	, ObjectConstants(make_shared<FRenderingResourcesUpdate>())
	, GeometryKey(0)
	, MeshObjectOffset(0)
	, MeshRenderingData(nullptr)
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
