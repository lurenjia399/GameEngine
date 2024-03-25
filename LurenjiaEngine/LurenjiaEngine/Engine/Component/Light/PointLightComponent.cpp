#include "PointLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"

CPointLightComponent::CPointLightComponent()
	: LightMeshComponent(nullptr)
{
	string path = "../LurenjiaEngine/Asset/PointMesh.obj";
	LightMeshComponent = GetMeshManage()->CreateCustomMeshComponent("SpotLightMeshComponent", path);
	if (LightMeshComponent)
	{
		shared_ptr<CMaterial> Material = LightMeshComponent->GetMaterials()[0];
		Material->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		Material->SetMaterialType(EMaterialType::BaseColor);
		Material->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
	}
	
	SetLightType(ELightType::PointLight);
}

void CPointLightComponent::SetPosition(const XMFLOAT3& InPosition)
{
	super::SetPosition(InPosition);
	LightMeshComponent->SetPosition(InPosition);
}

void CPointLightComponent::SetRotation(const fvector_3d& InRotation)
{
	super::SetRotation(InRotation);
	LightMeshComponent->SetRotation(InRotation);
}

void CPointLightComponent::SetScale(const XMFLOAT3& InScale)
{
	super::SetScale(InScale);
	LightMeshComponent->SetScale(InScale);
}

shared_ptr<CMeshComponent> CPointLightComponent::GetLightMeshComponent()
{
	return LightMeshComponent;
}