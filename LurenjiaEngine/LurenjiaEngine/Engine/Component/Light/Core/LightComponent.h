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
};

#endif // !CLightComponent
