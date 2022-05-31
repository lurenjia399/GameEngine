#include "PlaneMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"
#include "../Mesh/Core/MeshManage.h"

void APlaneMesh::Init()
{
	Super::Init();
}

void APlaneMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void APlaneMesh::SetMeshComponent(string InName, const float& InHeight, const float& InWidth, const uint32_t& InHeightSubdivide, const uint32_t& InWidthSubdivide)
{
	//给当前的mesh设置相应的meshComponent
	Super::SetMeshComponent(GetMeshManage()->CreatePlaneMeshComponent(InName, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide));
}

void APlaneMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
