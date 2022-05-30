#include "MeshComponent.h"

CMeshComponent::CMeshComponent()
{
	Materials.emplace_back(CreateObject<CMaterial>("Material"));
}

void CMeshComponent::Init()
{
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void CMeshComponent::SetSubMaterials(const int& index, CMaterial* InMaterial)
{
	Materials[index] = InMaterial;
}

UINT CMeshComponent::GetMaterialsCount() const
{
	return Materials.size();
}

const vector<CMaterial*>* CMeshComponent::GetMaterials() const
{
	return &Materials;
}