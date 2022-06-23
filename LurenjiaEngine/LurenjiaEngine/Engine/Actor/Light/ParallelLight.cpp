#include "ParallelLight.h"
#include "../../Manage/LightManage.h"
#include "../../Mesh/Core/MeshManage.h"

AParallelLight::AParallelLight()
	: ParallelLightComponent(nullptr)
{
	//灯光的基类CLightComponent会自动将component添加到LightManage中
	ParallelLightComponent = CreateObject<CParallelLightComponent>("ParallelLightComponent");
}

void AParallelLight::Tick(float DeltaTime)
{
	fvector_3d rotation = fvector_3d(0.f, DeltaTime * 40, 0.f);
	rotation = fvector_3d(DeltaTime * 180, 0.f, 0.f);
	SetRotation(rotation);
}

void AParallelLight::SetPosition(const XMFLOAT3& InPosition)
{
	ParallelLightComponent->SetPosition(InPosition);
}

void AParallelLight::SetRotation(const fvector_3d& InRotation)
{
	ParallelLightComponent->SetRotation(InRotation);
}

void AParallelLight::SetScale(const XMFLOAT3& InScale)
{
	ParallelLightComponent->SetScale(InScale);
}

void AParallelLight::SetLightIntensity(const XMFLOAT3& InLightIntensity)
{
	ParallelLightComponent->SetLightIntensity(InLightIntensity);
}

XMFLOAT3 AParallelLight::GetPosition() const
{
	return ParallelLightComponent->GetPosition();
}

fvector_3d AParallelLight::GetRotation() const
{
	return ParallelLightComponent->GetRotation();
}

XMFLOAT3 AParallelLight::GetScale() const
{
	return ParallelLightComponent->GetScale();
}

XMFLOAT3 AParallelLight::GetLightIntensity()
{
	return ParallelLightComponent->GetLightIntensity();
}

