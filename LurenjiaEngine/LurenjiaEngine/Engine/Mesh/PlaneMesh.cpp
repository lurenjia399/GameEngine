#include "PlaneMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Component/Mesh/PlaneMeshComponent.h"
#include "../Core/Construction/MacroConstruction.h"

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

	MeshComponent = LurenjiaEngine::CreateObject<CPlaneMeshComponent>(InName);

	INJECT_DATA_INTO_MESHCOMPONENT(CPlaneMeshComponent, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

void APlaneMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
