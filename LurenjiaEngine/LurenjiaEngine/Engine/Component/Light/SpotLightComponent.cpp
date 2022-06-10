#include "SpotLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"

CSpotLightComponent::CSpotLightComponent()
	: LightMeshComponent(nullptr)
	, ConicalInnerCorner(0.f)
	, ConicalOuterCorner(0.f)
{
	string path = "../LurenjiaEngine/Asset/SpotMesh.obj";
	LightMeshComponent = GetMeshManage()->CreateCustomMeshComponent("SpotLightMeshComponent", path);
	if (LightMeshComponent)
	{
		LightMeshComponent->SetRotation(fvector_3d(0.f, 180.f, 0.f));
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

void CSpotLightComponent::SetConicalInnerCorner(float InConicalInnerCorner)
{
	//�� ���õ��ڽ� ���� �������
	//�� ͬʱ�����ڽǺ���ǣ���һ����
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
	//�� ������� С�� �����ڽ�
	//�� ͬʱ�����ڽǺ���ǣ���һ����
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

