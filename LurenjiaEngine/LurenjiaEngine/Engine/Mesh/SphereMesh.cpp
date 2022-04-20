#include "SphereMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"

void CSphereMesh::Init()
{
	Super::Init();
}

void CSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

CSphereMesh* CSphereMesh::CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	FMeshRenderingData SphereMeshInfo;

	float horizontalAngle = XM_2PI / InAxialSubdivision;
	float verticalAngle = XM_PI / InHeightSubdivision;


	//创建顶点
	SphereMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(0, InRadius, 0), XMFLOAT4(Colors::BlueViolet)));//北极点
	for (uint32_t j = 1; j <= InHeightSubdivision;j++)
	{
		float vertical = j * verticalAngle;
		for (uint32_t i = 1; i <= InAxialSubdivision; i++)
		{
			float horizontal = i * horizontalAngle;
			float x = InRadius * sinf(vertical) * cosf(horizontal);
			float y = InRadius * cosf(vertical);
			float z = InRadius * sinf(vertical) * sinf(horizontal);
			SphereMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(x, y, z), XMFLOAT4(Colors::BlueViolet)));

			uint32_t currIndex = SphereMeshInfo.VertexData.size() - 1;

			XMVECTOR normal = XMLoadFloat3(&SphereMeshInfo.VertexData[currIndex].Pos);
			XMStoreFloat3(&SphereMeshInfo.VertexData[currIndex].Normal, normal);
		}
	}
	SphereMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(0, -InRadius, 0), XMFLOAT4(Colors::BlueViolet)));//南极点

	//创建索引
	for (uint32_t i = 1; i <= InAxialSubdivision;i++)
	{
		uint32_t finalIndex = (i == InAxialSubdivision) ? i - InAxialSubdivision + 1 : i + 1;
		SphereMeshInfo.IndexData.emplace_back(0);SphereMeshInfo.IndexData.emplace_back(finalIndex);SphereMeshInfo.IndexData.emplace_back(i);
	}
	for (uint32_t j = 1; j < InHeightSubdivision;j++)
	{
		for (uint32_t i = 1; i <= InAxialSubdivision; i++)
		{
			uint32_t left_up = i + (j - 1) * InAxialSubdivision;uint32_t right_up = (i == InAxialSubdivision) ? left_up - InAxialSubdivision + 1 : left_up + 1;
			uint32_t left_dowm = left_up + InAxialSubdivision;uint32_t right_dowm = right_up + InAxialSubdivision;
			SphereMeshInfo.IndexData.emplace_back(left_dowm);SphereMeshInfo.IndexData.emplace_back(left_up);SphereMeshInfo.IndexData.emplace_back(right_up);
			SphereMeshInfo.IndexData.emplace_back(left_dowm);SphereMeshInfo.IndexData.emplace_back(right_up);SphereMeshInfo.IndexData.emplace_back(right_dowm);
		}
	}
	for (uint32_t i = 1; i <= InAxialSubdivision;i++)
	{
		uint32_t left_up = i + (InHeightSubdivision - 1) * InAxialSubdivision;uint32_t right_up = (i == InAxialSubdivision) ? left_up - InAxialSubdivision + 1 : left_up + 1;
		SphereMeshInfo.IndexData.emplace_back(left_up);SphereMeshInfo.IndexData.emplace_back(right_up);SphereMeshInfo.IndexData.emplace_back(SphereMeshInfo.VertexData.size()-1);
	}


	CSphereMesh* SphereMesh = new CSphereMesh();
	SphereMesh->ResetGuid("SphereMesh");
	SphereMesh->BuildMesh(&SphereMeshInfo);
	

	SphereMesh->Init();
	return SphereMesh;
}

void CSphereMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
