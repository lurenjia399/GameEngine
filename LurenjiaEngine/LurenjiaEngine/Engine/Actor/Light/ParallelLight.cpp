#include "ParallelLight.h"
#include "../../Manage/LightManage.h"
#include "../../Mesh/Core/MeshManage.h"

AParallelLight::AParallelLight()
	: ParallelLightComponent(nullptr)
{
}

void AParallelLight::BeginInit()
{
	Super::BeginInit();

	//灯光的基类CLightComponent会自动将component添加到LightManage中
	ParallelLightComponent = LurenjiaEngine::CreateObject<CParallelLightComponent>(shared_from_this(), "ParallelLightComponent");
}

void AParallelLight::Tick(float DeltaTime)
{
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

void AParallelLight::SetRotateFunction(std::function<void(float)> RotateFunction)
{
	ParallelLightComponent->SetRotateFunction(RotateFunction);
}

void AParallelLight::SetLightIntensity(const XMFLOAT3& InLightIntensity)
{
	ParallelLightComponent->SetLightIntensity(InLightIntensity);
}

void AParallelLight::FaceTarget(const XMFLOAT3& InPosition, const XMFLOAT3& InTargetPosition, const XMFLOAT3& InUpDirection)
{
	ParallelLightComponent->FaceTarget(InPosition, InTargetPosition, InUpDirection);
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

