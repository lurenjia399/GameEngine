#include "BoxMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Component/Mesh/BoxMeshComponent.h"

void ABoxMesh::Init()
{
	Super::Init();
}

void ABoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ABoxMesh::SetMeshComponent(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision)
{
	MeshComponent = LurenjiaEngine::CreateObject<CBoxMeshComponent>(InName);

	INJECT_DATA_INTO_MESHCOMPONENT(CBoxMeshComponent, InRadius, InAxialSubdivision, InHeightSubdivision);
}

void ABoxMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
