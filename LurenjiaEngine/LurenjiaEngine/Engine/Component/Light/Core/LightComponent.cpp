#include "LightComponent.h"
#include "../../../Manage/LightManage.h"

CLightComponent::CLightComponent()
	: LightIntensity(1.f, 1.f, 1.f)
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

XMFLOAT3 CLightComponent::GetLightIntensity()
{
	return LightIntensity;
}
