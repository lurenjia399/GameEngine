#include "Light.h"

ALight::ALight()
	: LightComponent(nullptr)
{
}

void ALight::SetPosition(const XMFLOAT3& InPosition)
{
	LightComponent->SetPosition(InPosition);
}

void ALight::SetRotation(const fvector_3d& InRotation)
{
	LightComponent->SetRotation(InRotation);
}

void ALight::SetScale(const XMFLOAT3& InScale)
{
	LightComponent->SetScale(InScale);
}

XMFLOAT3 ALight::GetPosition() const
{
	return LightComponent ? LightComponent->GetPosition() : XMFLOAT3(0.f, 0.f, 1.f);
}

fvector_3d ALight::GetRotation() const
{
	return LightComponent ? LightComponent->GetRotation() : fvector_3d(0.f, 0.f, 1.f);
}

XMFLOAT3 ALight::GetScale() const
{
	return LightComponent ? LightComponent->GetScale() : XMFLOAT3(0.f, 0.f, 1.f);
}

XMFLOAT3 ALight::GetForward() const
{
	return LightComponent ? LightComponent->GetForward() : XMFLOAT3(0.f, 0.f, 1.f);
}

XMFLOAT3 ALight::GetRight() const
{
	return LightComponent ? LightComponent->GetRight() : XMFLOAT3(0.f, 0.f, 1.f);
}

XMFLOAT3 ALight::GetUp() const
{
	return LightComponent ? LightComponent->GetUp() : XMFLOAT3(0.f, 0.f, 1.f);
}
