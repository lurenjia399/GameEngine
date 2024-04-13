#include "SphereMeshComponent.h"

CSphereMeshComponent::CSphereMeshComponent()
{
}

void CSphereMeshComponent::CreateMeshRenderData(FVertexRenderingData& InRenderingData, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision)
{
	float horizontalAngle = XM_2PI / InAxialSubdivision;
	float verticalAngle = XM_PI / InHeightSubdivision;
	float TexCoordAngle = XM_2PI / (InAxialSubdivision - 1);

	//创建顶点
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(0, InRadius, 0), XMFLOAT4(Colors::Red), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f)));//北极点
	for (uint32_t j = 1; j < InHeightSubdivision;j++)
	{
		float vertical = j * verticalAngle;
		for (uint32_t i = 0; i <= InAxialSubdivision; i++)
		{
			float horizontal = i * horizontalAngle; 
			float x = InRadius * sinf(vertical) * cosf(horizontal);
			float y = InRadius * cosf(vertical);
			float z = InRadius * sinf(vertical) * sinf(horizontal);
			InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(x, y, z), XMFLOAT4(Colors::White)));

			uint32_t currIndex = static_cast<uint32_t>(InRenderingData.VertexData.size()) - 1;

			//顶点切线
			XMVECTOR normal = XMLoadFloat3(&InRenderingData.VertexData[currIndex].Pos);
			XMStoreFloat3(&InRenderingData.VertexData[currIndex].Normal, normal);

			XMFLOAT3 UTangent = XMFLOAT3(-x, 0, z); // 这里之前是XMFLOAT3(-z, 0, x)没有验证这次修改是否正确
			XMVECTOR Tangent = XMLoadFloat3(&UTangent);
			XMStoreFloat3(&InRenderingData.VertexData[currIndex].UTangent, Tangent);

			float v = vertical / XM_PI;
			float u = horizontal / XM_2PI;
			InRenderingData.VertexData[currIndex].TexCoord.x = u;
			InRenderingData.VertexData[currIndex].TexCoord.y = v;
		}
	}
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(0, -InRadius, 0), XMFLOAT4(Colors::Red), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT2(0.f, 0.5f)));//南极点

	//创建索引
	for (uint32_t i = 1; i <= InAxialSubdivision;i++)
	{
		uint32_t finalIndex = i + 1;
		InRenderingData.IndexData.emplace_back(0);InRenderingData.IndexData.emplace_back(finalIndex);InRenderingData.IndexData.emplace_back(i);
	}
	for (uint32_t j = 1; j < InHeightSubdivision - 1;j++)
	{
		for (uint32_t i = 1; i <= InAxialSubdivision; i++)
		{
			uint32_t left_up = i + (j - 1) * (InAxialSubdivision + 1);uint32_t right_up = left_up + 1;
			uint32_t left_dowm = left_up + (InAxialSubdivision + 1);uint32_t right_dowm = left_dowm + 1;
			InRenderingData.IndexData.emplace_back(left_dowm);InRenderingData.IndexData.emplace_back(left_up);InRenderingData.IndexData.emplace_back(right_up);
			InRenderingData.IndexData.emplace_back(left_dowm);InRenderingData.IndexData.emplace_back(right_up);InRenderingData.IndexData.emplace_back(right_dowm);
		}
	}
	for (uint32_t i = 1; i <= InAxialSubdivision;i++)
	{
		uint32_t left_up = i + (InHeightSubdivision - 2) * (InAxialSubdivision + 1);uint32_t right_up = left_up + 1;
		uint32_t left_down = static_cast<uint32_t>(InRenderingData.VertexData.size()) - 1;
		InRenderingData.IndexData.emplace_back(left_up);InRenderingData.IndexData.emplace_back(right_up);InRenderingData.IndexData.emplace_back(left_down);
	}
}

void CSphereMeshComponent::BuildKey(size_t& OutHashKey, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision)
{
	std::hash<float> floatHash;
	std::hash<int> intHash;

	OutHashKey = floatHash(InRadius);
	OutHashKey += intHash(InAxialSubdivision);
	OutHashKey += intHash(InHeightSubdivision);
}
