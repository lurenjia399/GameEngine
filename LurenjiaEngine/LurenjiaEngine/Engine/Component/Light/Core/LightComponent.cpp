#include "LightComponent.h"
#include "../../../Manage/LightManage.h"

CLightComponent::CLightComponent()
	: LightIntensity(1.f, 1.f, 1.f)
	, LightType(ELightType::ParallelLight)
{
	GetLightManage()->AddLightComponent(this);
}

CLightComponent::~CLightComponent()
{
	//
}

void CLightComponent::SetLightIntensity(XMFLOAT3 InLightIntensity)
{
	LightIntensity = InLightIntensity;
}

void CLightComponent::SetLightType(ELightType InLightType)
{
	LightType = InLightType;
}

XMFLOAT3 CLightComponent::GetLightIntensity()
{
	return LightIntensity;
}

ELightType CLightComponent::GetLightType()
{
	return LightType;
}
