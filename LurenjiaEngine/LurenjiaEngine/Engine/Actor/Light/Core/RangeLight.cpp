#include "RangeLight.h"

ARangeLight::ARangeLight()
	: RangeLightComponent(nullptr)
{
}

void ARangeLight::Tick(float DeltaTime)
{
}

void ARangeLight::SetPosition(const XMFLOAT3& InPosition)
{
	RangeLightComponent->SetPosition(InPosition);
}

void ARangeLight::SetRotation(const fvector_3d& InRotation)
{
	RangeLightComponent->SetRotation(InRotation);
}

void ARangeLight::SetScale(const XMFLOAT3& InScale)
{
	RangeLightComponent->SetScale(InScale);
}

void ARangeLight::SetLightIntensity(const XMFLOAT3& InLightIntensity)
{
	RangeLightComponent->SetLightIntensity(InLightIntensity);
}

void ARangeLight::SetLightStartAttenuation(const float& InStartAttenuation)
{
	RangeLightComponent->SetStartAttenuation(InStartAttenuation);
}

void ARangeLight::SetLightEndAttenuation(const float& InEndAttenuation)
{
	RangeLightComponent->SetEndAttenuation(InEndAttenuation);
}

XMFLOAT3 ARangeLight::GetPosition() const
{
	return RangeLightComponent ? RangeLightComponent->GetPosition() : XMFLOAT3(0.f, 0.f, 1.f);
}

fvector_3d ARangeLight::GetRotation() const
{
	return RangeLightComponent ? RangeLightComponent->GetRotation() : fvector_3d(0.f, 0.f, 1.f);
}

XMFLOAT3 ARangeLight::GetScale() const
{
	return RangeLightComponent ? RangeLightComponent->GetScale() : XMFLOAT3(0.f, 0.f, 1.f);
}

XMFLOAT3 ARangeLight::GetForward() const
{
	return RangeLightComponent ? RangeLightComponent->GetForward() : XMFLOAT3(0.f, 0.f, 1.f);
}

XMFLOAT3 ARangeLight::GetRight() const
{
	return RangeLightComponent ? RangeLightComponent->GetRight() : XMFLOAT3(0.f, 0.f, 1.f);
}

XMFLOAT3 ARangeLight::GetUp() const
{
	return RangeLightComponent ? RangeLightComponent->GetUp() : XMFLOAT3(0.f, 0.f, 1.f);
}

XMFLOAT3 ARangeLight::GetLightIntensity()
{
	return RangeLightComponent->GetLightIntensity();
}

float ARangeLight::GetLightStartAttenuation()
{
	return RangeLightComponent->GetStartAttenuation();
}

float ARangeLight::GetLightEndAttenuation()
{
	return RangeLightComponent->GetEndAttenuation();
}
