#include "SpotLightComponent.h"
#include "../../Actor/Mesh/Core/MeshManage.h"

CSpotLightComponent::CSpotLightComponent()
	: LightMeshComponent(nullptr)
	, ConicalInnerCorner(0.f)
	, ConicalOuterCorner(0.f)
{
	string path = "/SpotMesh.obj";
	string ContentPath = FEnginePathHelper::GetEngineContentPath();
	LightMeshComponent = GetMeshManage()->CreateCustomMeshComponent("SpotLightMeshComponent", ContentPath + path);
	if (LightMeshComponent)
	{
		shared_ptr<CMaterial> Material = LightMeshComponent->GetMaterials()[0];
		Material->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		Material->SetMaterialType(EMaterialType::BaseColor);
		Material->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
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

void CSpotLightComponent::SetConicalInnerCorner(float InConicalInnerCorner)
{
	//若 设置的内角 大于 现有外角
	//则 同时设置内角和外角，并一样大
	if (InConicalInnerCorner > ConicalOuterCorner)
	{
		ConicalInnerCorner = InConicalInnerCorner;
		ConicalOuterCorner = InConicalInnerCorner;
	}
	else {
		ConicalInnerCorner = InConicalInnerCorner;
	}
}

void CSpotLightComponent::SetConicalOuterCorner(float InConicalOuterCorner)
{
	//若 设置外角 小于 现有内角
	//则 同时设置内角和外角，并一样大
	if (InConicalOuterCorner < ConicalInnerCorner)
	{
		ConicalInnerCorner = InConicalOuterCorner;
		ConicalOuterCorner = InConicalOuterCorner;
	}
	else {
		ConicalOuterCorner = InConicalOuterCorner;
	}
	
}

float CSpotLightComponent::GetConicalInnerCorner() const
{
	return ConicalInnerCorner;
}

float CSpotLightComponent::GetConicalOuterCorner() const
{
	return ConicalOuterCorner;
}

