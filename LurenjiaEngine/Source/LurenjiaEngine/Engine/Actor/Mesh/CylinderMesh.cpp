#include "CylinderMesh.h"
#include "../../Component/Mesh/CylinderMeshComponent.h"
#include "../../Core/Construction/MacroConstruction.h"

void ACylinderMesh::Init()
{
	Super::Init();
}

void ACylinderMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ACylinderMesh::SetMeshComponent(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision, EMeshComponentRenderLayerType InType)
{
	//重新赋值meshcomponent，在构造amesh的时候就创建过meshComponent
	//如果meshComponent有值，则首先delete掉
	if (MeshComponent.get()) MeshComponent.reset();
	MeshComponent = LurenjiaEngine::CreateObject<CCylinderMeshComponent>(this, InName);
	MeshComponent->SetMeshComponentLayerType(InType);

	//给MeshComponet注入数据，使用前必须保证MeshComponet已经创建完毕
	//也就是需要保证meshComponent里面的参数都设定好（比如，层级等）
	InjectComponentRenderDataByMesh(CCylinderMeshComponent, InTopRadius, InBottomRadius, InAxialSubdivision, InHeight, InHeightSubdivision);
}

void ACylinderMesh::BuildMesh(const FVertexRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
