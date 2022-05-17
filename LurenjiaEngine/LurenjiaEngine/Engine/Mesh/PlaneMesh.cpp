#include "PlaneMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"

void APlaneMesh::Init()
{
	Super::Init();
}

void APlaneMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void APlaneMesh::CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide)
{
	std::function<float(float, uint32_t)> getStep = [&](const float& InValue, const uint32_t& InSubValue) ->float
	{
		if (InSubValue <= 0)
		{
			return InValue;
		}
		return InValue / InSubValue;
	};

	float pHeight = Inheight / 2;
	float pWidth = Inwidth / 2;

	float heightStep = getStep(Inheight, InHeightSubdivide);
	float widthStep = getStep(Inwidth, InwidthSubdivide);

	//平面的四个点
	XMFLOAT3 PivotPoint = XMFLOAT3(-pWidth, 0.0f, pHeight);

	for (uint32_t j = 0; j <= InHeightSubdivide; ++j)
	{
		for (uint32_t i = 0; i <= InwidthSubdivide; ++i)
		{
			float x = PivotPoint.x + i * widthStep;
			float y = PivotPoint.y;
			float z = PivotPoint.z - j * heightStep;
			InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(x, y, z), XMFLOAT4(Colors::White), XMFLOAT3(0.f, 1.f, 0.0f)));
			
		}
	}

	for (uint32_t j = 0; j < InHeightSubdivide; j++)
	{
		for (uint32_t i = 0; i < InwidthSubdivide; i++)
		{
			uint32_t leftUp = i + j * (InwidthSubdivide + 1);uint32_t rightUp = leftUp + 1;
			uint32_t leftDown = leftUp + (InwidthSubdivide + 1);uint32_t rightDown = leftDown + 1;
			InRenderingData.IndexData.emplace_back(leftDown);InRenderingData.IndexData.emplace_back(leftUp);InRenderingData.IndexData.emplace_back(rightUp);
			InRenderingData.IndexData.emplace_back(leftDown);InRenderingData.IndexData.emplace_back(rightUp);InRenderingData.IndexData.emplace_back(rightDown);
		}
	}
}

void APlaneMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
