#include "LightComponent.h"
#include "../../../Manage/LightManage.h"

CLightComponent::CLightComponent()
	: LightIntensity(1.f, 1.f, 1.f)
	, LightType(ELightType::ParallelLight)
	, RotateFunction(nullptr)
{
	GetLightManage()->AddLightComponent(this);
}

CLightComponent::~CLightComponent()
{
	//
}

void CLightComponent::Tick(float DeltaTime)
{
	if(RotateFunction != nullptr)
	{
		RotateFunction(DeltaTime);
	}
	
}

void CLightComponent::SetLightIntensity(XMFLOAT3 InLightIntensity)
{
	LightIntensity = InLightIntensity;
}

void CLightComponent::SetLightType(ELightType InLightType)
{
	LightType = InLightType;
}

void CLightComponent::SetRotateFunction(std::function<void(float)> InRotateFunction)
{
	RotateFunction = InRotateFunction;
}

void CLightComponent::FaceTarget(const XMFLOAT3& InPosition, const XMFLOAT3& InTargetPosition, const XMFLOAT3& InUpDirection)
{
	XMVECTOR forward = XMVectorSubtract(XMLoadFloat3(&InTargetPosition), XMLoadFloat3(&InPosition));
	forward = XMVector3Normalize(forward);

	XMVECTOR up = XMLoadFloat3(&InUpDirection);
	up = XMVector3Normalize(up);

	XMVECTOR right = XMVector3Cross(up, forward);
	right = XMVector3Normalize(right);

	// 重新矫正up向量
	XMVECTOR up_new = XMVector3Cross(forward, right);
	up_new = XMVector3Normalize(up);

	SetForward(XMFLOAT3(forward.m128_f32[0], forward.m128_f32[1], forward.m128_f32[2]));
	SetRight(XMFLOAT3(right.m128_f32[0], right.m128_f32[1], right.m128_f32[2]));
	SetUp(XMFLOAT3(up_new.m128_f32[0], up_new.m128_f32[1], up_new.m128_f32[2]));

}

XMFLOAT3 CLightComponent::GetLightIntensity()
{
	return LightIntensity;
}

ELightType CLightComponent::GetLightType()
{
	return LightType;
}
