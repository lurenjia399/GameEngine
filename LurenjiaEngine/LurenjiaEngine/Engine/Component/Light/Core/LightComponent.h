#pragma once
#include "../../TransformationComponent.h"

#ifndef LightComponent_H
#define LightComponent_H

#include "../../../Interface/DirectXDeviceInterface.h"
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "../../../Component/Light/Core/LightType.h"

class CLightComponent : public CTransformationComponent, public IDirectXDeviceInterface
{
public:
	CLightComponent();
	~CLightComponent();

	void SetLightIntensity(XMFLOAT3 InLightIntensity);
	void SetLightType(ELightType InLightType);

	XMFLOAT3 GetLightIntensity();
	ELightType GetLightType();

private:
	XMFLOAT3 LightIntensity;
	ELightType LightType;

};

#endif // !CLightComponent
