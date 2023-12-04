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
		XMFLOAT3 alias = RotateFunction(DeltaTime);
		XMFLOAT3 cur_pos = GetPosition();
		Engine_Log("cur_pos = %f, %f, %f", cur_pos.x, cur_pos.y, cur_pos.z);
		XMFLOAT3 new_pos = XMFLOAT3(cur_pos.x + alias.x, cur_pos.y + alias.y, cur_pos.z + alias.z);
		Engine_Log("new_pos = %f, %f, %f", new_pos.x, new_pos.y, new_pos.z);
		SetPosition(alias);
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

void CLightComponent::SetRotateFunction(std::function<XMFLOAT3(float)> InRotateFunction)
{
	RotateFunction = InRotateFunction;
}

XMFLOAT3 CLightComponent::GetLightIntensity()
{
	return LightIntensity;
}

ELightType CLightComponent::GetLightType()
{
	return LightType;
}
