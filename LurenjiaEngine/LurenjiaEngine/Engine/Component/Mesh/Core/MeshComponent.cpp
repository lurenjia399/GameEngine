#include "MeshComponent.h"

CMeshComponent::CMeshComponent()
	: MeshCompLayerType(EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE)
{
	Materials.emplace_back(LurenjiaEngine::CreateObject<CMaterial>(this, "CMeshComponent::Material"));
}

void CMeshComponent::Init()
{
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void CMeshComponent::SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterial)
{
	Materials[index] = InMaterial;

}

UINT CMeshComponent::GetMaterialsCount() const
{
	return static_cast<UINT>(Materials.size());
}

const vector<shared_ptr<CMaterial>> CMeshComponent::GetMaterials() const
{
	return Materials;
}

bool CMeshComponent::GetbIsDynamicReflection() const
{
	
	if (Materials.size() > 0)
	{
		for (std::shared_ptr<CMaterial> Material : Materials)
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
