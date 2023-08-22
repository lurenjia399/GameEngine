#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"

APointLight::APointLight()
	: time(0)
{
	RangeLightComponent = LurenjiaEngine::CreateObject<CPointLightComponent>("PointLightComponent");
}

void APointLight::Tick(float DeltaTime)
{
	time += DeltaTime;

	fvector_3d rotation = fvector_3d(0.f, DeltaTime * 40, 0.f);
	rotation = fvector_3d(0.f, DeltaTime * 180, DeltaTime * 180);
	//SetRotation(rotation);

	XMFLOAT3 location = RangeLightComponent->GetPosition();
	
	location.y += cos(time) * 0.3f;
	SetPosition(location);
}
