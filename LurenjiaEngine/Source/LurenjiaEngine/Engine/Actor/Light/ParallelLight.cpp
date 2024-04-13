#include "ParallelLight.h"
#include "../../Actor/Light/Core/LightManage.h"
#include "../../Actor/Mesh/Core/MeshManage.h"

AParallelLight::AParallelLight()
{
	//�ƹ�Ļ���CLightComponent���Զ���component��ӵ�LightManage��
	LightComponent = LurenjiaEngine::CreateObject<CParallelLightComponent>(this, "ParallelLightComponent");
}

void AParallelLight::SetRotateFunction(std::function<void(float)> RotateFunction)
{
	LightComponent->SetRotateFunction(RotateFunction);
}

void AParallelLight::FaceTarget(const XMFLOAT3& InPosition, const XMFLOAT3& InTargetPosition, const XMFLOAT3& InUpDirection)
{
	LightComponent->FaceTarget(InPosition, InTargetPosition, InUpDirection);
}
