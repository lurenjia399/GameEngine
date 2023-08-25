#include "MeshComponent.h"

CMeshComponent::CMeshComponent()
	: MeshCompLayerType(EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE)
{
	Materials.emplace_back(LurenjiaEngine::CreateObject<CMaterial>("CMeshComponent::Material"));
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

bool CMeshComponent::GetbIsDynamicReflection() const
{
	
	if (Materials.size() > 0)
	{
		for (CMaterial* Material : Materials)
		{
			return Material->GetbIsDynamicReflection();
		}
	}
	return false;
}

void CMeshComponent::SetMeshComponentLayerType(EMeshComponentRenderLayerType InType)
{
	MeshCompLayerType = InType;
}

EMeshComponentRenderLayerType CMeshComponent::GetMeshComponentLayerType()
{
	return MeshCompLayerType;
}
