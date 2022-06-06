#include "SpotLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"

CSpotLightComponent::CSpotLightComponent()
	: LightMeshComponent(nullptr)
	, StartAttenuation(0.f)
	, EndAttenuation(0.f)
{
	string path = "../LurenjiaEngine/Asset/SpotMesh.obj";
	LightMeshComponent = GetMeshManage()->CreateCustomMeshComponent("SpotLightMeshComponent", path);
	if (LightMeshComponent)
	{
		CMaterial* Material = (*LightMeshComponent->GetMaterials())[0];
		if (Material)
		{
			Material->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
			Material->SetMaterialType(EMaterialType::BaseColor);
			Material->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
		}
	}
	
	SetLightType(ELightType::SpotLight);
}

void CSpotLightComponent::SetPosition(const XMFLOAT3& InPosition)
{
	super::SetPosition(InPosition);
	LightMeshComponent->SetPosition(InPosition);
}

void CSpotLightComponent::SetRotation(const fvector_3d& InRotation)
{
	super::SetRotation(InRotation);
	LightMeshComponent->SetRotation(InRotation);
}

void CSpotLightComponent::SetScale(const XMFLOAT3& InScale)
{
	super::SetScale(InScale);
	LightMeshComponent->SetScale(InScale);
}

void CSpotLightComponent::SetStartAttenuation(float InStartAttenuation)
{
	StartAttenuation = InStartAttenuation;
}

void CSpotLightComponent::SetEndAttenuation(float InEndAttenuation)
{
	EndAttenuation = InEndAttenuation;
}

float CSpotLightComponent::GetStartAttenuation()
{
	return StartAttenuation;
}

float CSpotLightComponent::GetEndAttenuation()
{
	return EndAttenuation;
}
