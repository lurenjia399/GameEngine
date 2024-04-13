#include "SphereMesh.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../Mesh/Core/MeshManage.h"
#include "../../Core/Construction/MacroConstruction.h"
#include "../../Component/Mesh/SphereMeshComponent.h"

void ASphereMesh::Init()
{
	Super::Init();
}

void ASphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ASphereMesh::SetMeshComponent(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision, EMeshComponentRenderLayerType InType)
{
	//重新赋值meshcomponent，在构造amesh的时候就创建过meshComponent
	//如果meshComponent有值，则首先delete掉
	if (MeshComponent.get()) MeshComponent.reset();
	MeshComponent = LurenjiaEngine::CreateObject<CSphereMeshComponent>(this, InName);
	MeshComponent->SetMeshComponentLayerType(InType);

	//给MeshComponet注入数据，使用前必须保证MeshComponet已经创建完毕
	//也就是需要保证meshComponent里面的参数都设定好（比如，层级等）
	INJECT_DATA_INTO_MESHCOMPONENT(CSphereMeshComponent, InRadius, InAxialSubdivision, InHeightSubdivision)

}

void ASphereMesh::BuildMesh(const FVertexRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
