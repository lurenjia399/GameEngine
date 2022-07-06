#include "SpotLight.h"

ASpotLight::ASpotLight()
{
	RangeLightComponent = LurenjiaEngine::CreateObject<CSpotLightComponent>("PointLightComponent");
}

void ASpotLight::Tick(float DeltaTime)
{
	fvector_3d rotation = fvector_3d(0.f, DeltaTime * 240, 0.f);
	//rotation = fvector_3d(0.f, DeltaTime * 180, DeltaTime * 180);
	SetRotation(rotation);
	XMFLOAT3 location = RangeLightComponent->GetPosition();
	location.z -= 0.05f;
	//SetPosition(location);
}

void ASpotLight::SetConicalInnerCorner(float InConicalInnerCorner)
{
	if (CSpotLightComponent* SpotLightComponent = dynamic_cast<CSpotLightComponent*>(RangeLightComponent))
	{
		SpotLightComponent->SetConicalInnerCorner(InConicalInnerCorner);
	}
}

void ASpotLight::SetConicalOuterCorner(float InConicalOuterCorner)
{
	if (CSpotLightComponent* SpotLightComponent = dynamic_cast<CSpotLightComponent*>(RangeLightComponent))
	{
		SpotLightComponent->SetConicalOuterCorner(InConicalOuterCorner);
	}
}

float ASpotLight::GetConicalInnerCorner() const
{
	if (CSpotLightComponent* SpotLightComponent = dynamic_cast<CSpotLightComponent*>(RangeLightComponent))
	{
		SpotLightComponent->GetConicalInnerCorner();
	}
	return -1.f;
}

float ASpotLight::GetConicalOuterCorner() const
{
	if (CSpotLightComponent* SpotLightComponent = dynamic_cast<CSpotLightComponent*>(RangeLightComponent))
	{
		SpotLightComponent->GetConicalOuterCorner();
	}
	return -1.f;
}
