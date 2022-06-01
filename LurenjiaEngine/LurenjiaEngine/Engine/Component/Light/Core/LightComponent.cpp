#include "LightComponent.h"
#include "../../../Manage/LightManage.h"

CLightComponent::CLightComponent()
{
	GetLightManage()->AddLightComponent(this);
}

CLightComponent::~CLightComponent()
{
	//
}
