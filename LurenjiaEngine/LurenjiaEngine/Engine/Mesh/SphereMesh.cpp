#include "SphereMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/SphereMeshComponent.h"

void ASphereMesh::Init()
{
	Super::Init();
}

void ASphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ASphereMesh::SetMeshComponent(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision)
{
	//首先创建MeshComponet
	MeshComponent = LurenjiaEngine::CreateObject<CSphereMeshComponent>(InName);
	//给MeshComponet注入数据，使用前必须保证MeshComponet已经创建完毕
	INJECT_DATA_INTO_MESHCOMPONENT(CSphereMeshComponent, InRadius, InAxialSubdivision, InHeightSubdivision)

}

void ASphereMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
