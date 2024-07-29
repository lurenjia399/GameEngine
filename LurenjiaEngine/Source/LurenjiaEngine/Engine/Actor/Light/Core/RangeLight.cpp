#include "RangeLight.h"

ARangeLight::ARangeLight()
{
}


void ARangeLight::SetLightStartAttenuation(const float& InStartAttenuation)
{
	CRangeLightComponent* RangeLightComponent = static_cast<CRangeLightComponent*>(LightComponent);
	if (RangeLightComponent)
	{
		RangeLightComponent->SetStartAttenuation(InStartAttenuation);
	}
	
}

void ARangeLight::SetLightEndAttenuation(const float& InEndAttenuation)
{
	CRangeLightComponent* RangeLightComponent = static_cast<CRangeLightComponent*>(LightComponent);
	if (RangeLightComponent)
	{
		RangeLightComponent->SetEndAttenuation(InEndAttenuation);
	}
}

float ARangeLight::GetLightStartAttenuation()
{
	CRangeLightComponent* RangeLightComponent = static_cast<CRangeLightComponent*>(LightComponent);
	if (RangeLightComponent)
	{
		return RangeLightComponent->GetStartAttenuation();
	}
	return 0.f;

}

float ARangeLight::GetLightEndAttenuation()
{
	CRangeLightComponent* RangeLightComponent = static_cast<CRangeLightComponent*>(LightComponent);
	if (RangeLightComponent)
	{
		return RangeLightComponent->GetEndAttenuation();
	}
	return 0.f;
}
