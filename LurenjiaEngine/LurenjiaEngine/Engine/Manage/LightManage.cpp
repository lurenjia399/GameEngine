#include "LightManage.h"

void CLightManage::AddLightComponent(CLightComponent* InLightComponent)
{
	Lights.emplace_back(InLightComponent);
}
