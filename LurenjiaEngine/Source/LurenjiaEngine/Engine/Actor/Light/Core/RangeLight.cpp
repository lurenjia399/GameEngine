#include "RangeLight.h"

ARangeLight::ARangeLight()
{
}


void ARangeLight::SetLightStartAttenuation(const float& InStartAttenuation)
{
	std::shared_ptr<CRangeLightComponent> RangeLightComponent = static_pointer_cast<CRangeLightComponent>(LightComponent);
	if (RangeLightComponent)
	{
		RangeLightComponent->SetStartAttenuation(InStartAttenuation);
	}
	
}

void ARangeLight::SetLightEndAttenuation(const float& InEndAttenuation)
{
	std::shared_ptr<CRangeLightComponent> RangeLightComponent = static_pointer_cast<CRangeLightComponent>(LightComponent);
	if (RangeLightComponent)
	{
		RangeLightComponent->SetEndAttenuation(InEndAttenuation);
	}
}

float ARangeLight::GetLightStartAttenuation()
{
	std::shared_ptr<CRangeLightComponent> RangeLightComponent = static_pointer_cast<CRangeLightComponent>(LightComponent);
	if (RangeLightComponent)
	{
		return RangeLightComponent->GetStartAttenuation();
	}
	return 0.f;

}

float ARangeLight::GetLightEndAttenuation()
{
	std::shared_ptr<CRangeLightComponent> RangeLightComponent = static_pointer_cast<CRangeLightComponent>(LightComponent);
	if (RangeLightComponent)
	{
		return RangeLightComponent->GetEndAttenuation();
	}
	return 0.f;
}
