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

void APlaneMesh::SetMeshComponent(string InName, const float& InHeight, const float& InWidth, const uint32_t& InHeightSubdivide, const uint32_t& InWidthSubdivide, EMeshComponentRenderLayerType InType)
{
	//重新赋值meshcomponent，在构造amesh的时候就创建过meshComponent
	//如果meshComponent有值，则首先delete掉
	if (MeshComponent) delete MeshComponent;
	MeshComponent = LurenjiaEngine::CreateObject<CPlaneMeshComponent>(InName);
	MeshComponent->SetMeshComponentLayerType(InType);

	//给MeshComponet注入数据，使用前必须保证MeshComponet已经创建完毕
	//也就是需要保证meshComponent里面的参数都设定好（比如，层级等）
	INJECT_DATA_INTO_MESHCOMPONENT(CPlaneMeshComponent, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

void APlaneMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
