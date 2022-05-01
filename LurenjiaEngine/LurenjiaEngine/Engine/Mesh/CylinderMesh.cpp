#include "CylinderMesh.h"

void ACylinderMesh::Init()
{
	Super::Init();
}

void ACylinderMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ACylinderMesh::CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision)
{
	float horizontalAngle = XM_2PI / InAxialSubdivision;
	float perHeight = InHeight / InHeightSubdivision;
	float otherTringleHeight = 1;
	uint32_t type = 0;//0 圆台 ，1 圆柱 ，2 正圆锥 3 倒圆锥
	if (InTopRadius == InBottomRadius )
	{
		type = 1;
	}
	else if (InTopRadius == 0)
	{
		type = 2;
	}
	else if (InBottomRadius == 0)
	{
		type = 3;
	}
	else {
		otherTringleHeight = InTopRadius * InHeight / (InBottomRadius - InTopRadius);
	}

	//构建顶顶点
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(0, InHeight, 0), XMFLOAT4(Colors::White)));
	for (uint32_t i = 1; i <= InAxialSubdivision; i++)
	{
		float x = InTopRadius * cosf(i * horizontalAngle);
		float y = InHeight;
		float z = InTopRadius * sinf(i * horizontalAngle);
		InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(x, y, z), XMFLOAT4(Colors::White)));
	}
	//构建身体顶点
	for (uint32_t i = 1; i <= InHeightSubdivision; i++)
	{
		float currRadius = 1.0f;
		if (type == 1 )
		{
			currRadius = InTopRadius;
		}
		else if (type == 2)
		{
			currRadius = i * perHeight / InHeight * InBottomRadius;
		}
		else if (type == 3)
		{
			currRadius = (InHeight - i * perHeight) / InHeight * InTopRadius;
		}
		else {
			currRadius = InTopRadius * (otherTringleHeight + i * perHeight) / otherTringleHeight;
		}
		for (uint32_t j = 1; j <= InAxialSubdivision; j++)
		{
			float x = currRadius * cosf(j * horizontalAngle);
			float y = InHeight - i * perHeight;
			float z = currRadius * sinf(j * horizontalAngle);
			InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(x, y, z), XMFLOAT4(Colors::White)));
		}
	}
	//构建底顶点
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(Colors::White)));

	//构建顶索引
	for (uint32_t i = 1; i <= InAxialSubdivision; i++)
	{
		uint32_t nextIndex = (i == InAxialSubdivision) ? i - InAxialSubdivision + 1 : i + 1;
		InRenderingData.IndexData.emplace_back(i);InRenderingData.IndexData.emplace_back(0);InRenderingData.IndexData.emplace_back(nextIndex);
	}
	//构建身体索引
	for (uint32_t i = 1; i <= InHeightSubdivision ; i++)
	{
		for (uint32_t j = 1; j <= InAxialSubdivision; j++)
		{
			uint32_t left_up = j + (i - 1) * InAxialSubdivision;uint32_t right_up = (j == InAxialSubdivision) ? left_up - InAxialSubdivision + 1 : left_up + 1;
			uint32_t left_dowm = left_up + InAxialSubdivision;uint32_t right_dowm = right_up + InAxialSubdivision;
			InRenderingData.IndexData.emplace_back(left_dowm);InRenderingData.IndexData.emplace_back(left_up);InRenderingData.IndexData.emplace_back(right_up);
			InRenderingData.IndexData.emplace_back(left_dowm);InRenderingData.IndexData.emplace_back(right_up);InRenderingData.IndexData.emplace_back(right_dowm);
		}
	}
	//构建底索引
	for (uint32_t i = 1; i <= InAxialSubdivision; i++)
	{
		uint32_t left_up = i + InHeightSubdivision * InAxialSubdivision;uint32_t right_up = (i == InAxialSubdivision) ? left_up - InAxialSubdivision + 1 : left_up + 1;
		InRenderingData.IndexData.emplace_back(InRenderingData.VertexData.size() - 1);InRenderingData.IndexData.emplace_back(left_up);InRenderingData.IndexData.emplace_back(right_up);
	}
}

void ACylinderMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
