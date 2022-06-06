#include "SpotLight.h"
#include "../../Mesh/Core/MeshManage.h"

ASpotLight::ASpotLight()
	: SpotLightComponent(nullptr)
{
	SpotLightComponent = CreateObject<CSpotLightComponent>("PointLightComponent");
}

void ASpotLight::Tick(float DeltaTime)
{
}

void ASpotLight::SetPosition(const XMFLOAT3& InPosition)
{
	SpotLightComponent->SetPosition(InPosition);
}

void ASpotLight::SetRotation(const fvector_3d& InRotation)
{
	SpotLightComponent->SetRotation(InRotation);
}

void ASpotLight::SetScale(const XMFLOAT3& InScale)
{
	SpotLightComponent->SetScale(InScale);
}

void ASpotLight::SetLightIntensity(const XMFLOAT3& InLightIntensity)
{
	SpotLightComponent->SetLightIntensity(InLightIntensity);
}

void ASpotLight::SetLightStartAttenuation(const float& InStartAttenuation)
{
	SpotLightComponent->SetStartAttenuation(InStartAttenuation);
}

void ASpotLight::SetLightEndAttenuation(const float& InEndAttenuation)
{
	SpotLightComponent->SetEndAttenuation(InEndAttenuation);
}

XMFLOAT3 ASpotLight::GetPosition() const
{
	return SpotLightComponent->GetPosition();
}

fvector_3d ASpotLight::GetRotation() const
{
	return SpotLightComponent->GetRotation();
}

XMFLOAT3 ASpotLight::GetScale() const
{
	return SpotLightComponent->GetScale();
}

XMFLOAT3 ASpotLight::GetForward() const
{
	return SpotLightComponent->GetForward();
}

XMFLOAT3 ASpotLight::GetRight() const
{
	return SpotLightComponent->GetRight();
}

XMFLOAT3 ASpotLight::GetUp() const
{
	return SpotLightComponent->GetUp();
}

XMFLOAT3 ASpotLight::GetLightIntensity()
{
	return SpotLightComponent->GetLightIntensity();
}

float ASpotLight::GetLightStartAttenuation()
{
	return SpotLightComponent->GetStartAttenuation();
}

float ASpotLight::GetLightEndAttenuation()
{
	return SpotLightComponent->GetEndAttenuation();
}
