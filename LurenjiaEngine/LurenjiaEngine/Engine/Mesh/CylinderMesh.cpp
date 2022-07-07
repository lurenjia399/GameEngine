#include "CylinderMesh.h"
#include "../Component/Mesh/CylinderMeshComponent.h"
#include "../Core/Construction/MacroConstruction.h"

void ACylinderMesh::Init()
{
	Super::Init();
}

void ACylinderMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void ACylinderMesh::SetMeshComponent(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision)
{
	MeshComponent = LurenjiaEngine::CreateObject<CCylinderMeshComponent>(InName);

	INJECT_DATA_INTO_MESHCOMPONENT(CCylinderMeshComponent, InTopRadius, InBottomRadius, InAxialSubdivision, InHeight, InHeightSubdivision);
}

void ACylinderMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
