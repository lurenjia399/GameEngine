#include "SpotLight.h"

ASpotLight::ASpotLight()
{
	
}

void ASpotLight::BeginInit()
{
	Super::BeginInit();
	RangeLightComponent = LurenjiaEngine::CreateObject<CSpotLightComponent>(shared_from_this(), "PointLightComponent");
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
	if (shared_ptr<CSpotLightComponent> SpotLightComponent = static_pointer_cast<CSpotLightComponent>(RangeLightComponent))
	{
		SpotLightComponent->SetConicalInnerCorner(InConicalInnerCorner);
	}
}

void ASpotLight::SetConicalOuterCorner(float InConicalOuterCorner)
{
	if (shared_ptr<CSpotLightComponent> SpotLightComponent = static_pointer_cast<CSpotLightComponent>(RangeLightComponent))
	{
		SpotLightComponent->SetConicalOuterCorner(InConicalOuterCorner);
	}
}

float ASpotLight::GetConicalInnerCorner() const
{
	if (shared_ptr<CSpotLightComponent> SpotLightComponent = static_pointer_cast<CSpotLightComponent>(RangeLightComponent))
	{
		SpotLightComponent->GetConicalInnerCorner();
	}
	return -1.f;
}

float ASpotLight::GetConicalOuterCorner() const
{
	if (shared_ptr<CSpotLightComponent> SpotLightComponent = static_pointer_cast<CSpotLightComponent>(RangeLightComponent))
	{
		SpotLightComponent->GetConicalOuterCorner();
	}
	return -1.f;
}
