#include "RangeLightComponent.h"

CRangeLightComponent::CRangeLightComponent()
	: StartAttenuation(0.f)
	, EndAttenuation(0.f)
{
}

void CRangeLightComponent::SetStartAttenuation(float InStartAttenuation)
{
	StartAttenuation = InStartAttenuation;
}

void CRangeLightComponent::SetEndAttenuation(float InEndAttenuation)
{
	EndAttenuation = InEndAttenuation;
}

float CRangeLightComponent::GetStartAttenuation()
{
	return StartAttenuation;
}

float CRangeLightComponent::GetEndAttenuation()
{
	return EndAttenuation;
}
