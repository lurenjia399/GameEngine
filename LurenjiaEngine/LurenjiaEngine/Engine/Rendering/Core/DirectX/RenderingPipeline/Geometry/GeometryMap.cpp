#include "GeometryMap.h"

FGeometryMap::FGeometryMap()
{
	Geometrys.emplace(0, FGeometry());
}

void FGeometryMap::BuildMesh(CMesh* InMesh, const FMeshRenderingData* InRenderingData)
{
	FGeometry& Geometry = Geometrys[0];
	Geometry.BuildMesh(InMesh, InRenderingData);
}

bool FGeometry::isExitRenderingData(CMesh* InKey)
{
	for (FRenderingData& data : DescribeMeshRenderingData)
	{
		if (data.Mesh == InKey)
		{
			return true;
		}
	}
	return false;
}

void FGeometry::BuildMesh(CMesh* InMesh, const FMeshRenderingData* MeshRenderData)
{
	if (!isExitRenderingData(InMesh))
	{
		DescribeMeshRenderingData.emplace_back(FRenderingData());
		FRenderingData& InRenderingData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];
		InRenderingData.Mesh = InMesh;
		InRenderingData.IndexSize = MeshRenderData->IndexData.size();
		InRenderingData.IndexoffsetPosition = MeshRenderingData.IndexData.size();
		InRenderingData.VertexSize = MeshRenderData->VertexData.size();
		InRenderingData.VertexoffsetPostion = MeshRenderingData.VertexData.size();

		MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshRenderData->IndexData.begin(), MeshRenderData->IndexData.end());
		MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshRenderData->VertexData.begin(), MeshRenderData->VertexData.end());

	}
}
