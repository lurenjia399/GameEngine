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

void CSphereMesh::CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision)
{
	float horizontalAngle = XM_2PI / InAxialSubdivision;
	float verticalAngle = XM_PI / InHeightSubdivision;

	//��������
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(0, InRadius, 0), XMFLOAT4(Colors::BlueViolet)));//������
	for (uint32_t j = 1; j <= InHeightSubdivision;j++)
	{
		float vertical = j * verticalAngle;
		for (uint32_t i = 1; i <= InAxialSubdivision; i++)
		{
			float horizontal = i * horizontalAngle;
			float x = InRadius * sinf(vertical) * cosf(horizontal);
			float y = InRadius * cosf(vertical);
			float z = InRadius * sinf(vertical) * sinf(horizontal);
			InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(x, y, z), XMFLOAT4(Colors::BlueViolet)));

			uint32_t currIndex = InRenderingData.VertexData.size() - 1;

			XMVECTOR normal = XMLoadFloat3(&InRenderingData.VertexData[currIndex].Pos);
			XMStoreFloat3(&InRenderingData.VertexData[currIndex].Normal, normal);
		}
	}
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(0, -InRadius, 0), XMFLOAT4(Colors::BlueViolet)));//�ϼ���

	//��������
	for (uint32_t i = 1; i <= InAxialSubdivision;i++)
	{
		uint32_t finalIndex = (i == InAxialSubdivision) ? i - InAxialSubdivision + 1 : i + 1;
		InRenderingData.IndexData.emplace_back(0);InRenderingData.IndexData.emplace_back(finalIndex);InRenderingData.IndexData.emplace_back(i);
	}
	for (uint32_t j = 1; j < InHeightSubdivision;j++)
	{
		for (uint32_t i = 1; i <= InAxialSubdivision; i++)
		{
			uint32_t left_up = i + (j - 1) * InAxialSubdivision;uint32_t right_up = (i == InAxialSubdivision) ? left_up - InAxialSubdivision + 1 : left_up + 1;
			uint32_t left_dowm = left_up + InAxialSubdivision;uint32_t right_dowm = right_up + InAxialSubdivision;
			InRenderingData.IndexData.emplace_back(left_dowm);InRenderingData.IndexData.emplace_back(left_up);InRenderingData.IndexData.emplace_back(right_up);
			InRenderingData.IndexData.emplace_back(left_dowm);InRenderingData.IndexData.emplace_back(right_up);InRenderingData.IndexData.emplace_back(right_dowm);
		}
	}
	for (uint32_t i = 1; i <= InAxialSubdivision;i++)
	{
		uint32_t left_up = i + (InHeightSubdivision - 1) * InAxialSubdivision;uint32_t right_up = (i == InAxialSubdivision) ? left_up - InAxialSubdivision + 1 : left_up + 1;
		InRenderingData.IndexData.emplace_back(left_up);InRenderingData.IndexData.emplace_back(right_up);InRenderingData.IndexData.emplace_back(InRenderingData.VertexData.size() - 1);
	}
}

void CSphereMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}