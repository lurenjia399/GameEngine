#pragma once
#include "../../TransformationComponent.h"

#ifndef LightComponent
#define LightComponent

#include "../../../Interface/DirectXDeviceInterface.h"
#include "../../../Core/CoreObject/CoreMinimalObject.h"

class CLightComponent : public CTransformationComponent, public IDirectXDeviceInterface
{
public:
	CLightComponent();
	~CLightComponent();

	void SetLightIntensity(XMFLOAT3 InLightIntensity);
	XMFLOAT3 GetLightIntensity();
private:
	XMFLOAT3 LightIntensity;
};

#endif // !CLightComponent
