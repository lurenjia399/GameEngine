#include "RenderingData.h"

FRenderingData::FRenderingData()
	: IndexSize(0)
	, VertexSize(0)
	, IndexoffsetPosition(0)
	, VertexoffsetPostion(0)
	, IndexTypeSize(sizeof(uint16_t))
	, VertexTypeSize(sizeof(FVertex))
{
}

UINT FRenderingData::GetVertexSizeInBytes() const
{
	return VertexSize * VertexTypeSize;
}

UINT FRenderingData::GetIndexSizeInBytes() const
{
	return IndexSize * IndexTypeSize;
}
