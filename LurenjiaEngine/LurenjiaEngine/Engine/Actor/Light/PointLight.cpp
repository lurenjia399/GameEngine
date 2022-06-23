#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"

APointLight::APointLight()
{
	RangeLightComponent = CreateObject<CPointLightComponent>("PointLightComponent");
}

void APointLight::Tick(float DeltaTime)
{
	fvector_3d rotation = fvector_3d(0.f, DeltaTime * 40, 0.f);
	rotation = fvector_3d(0.f, DeltaTime * 180, DeltaTime * 180);
	SetRotation(rotation);
	XMFLOAT3 location = RangeLightComponent->GetPosition();
	location.z -= 0.05f;
	//SetPosition(location);
}
