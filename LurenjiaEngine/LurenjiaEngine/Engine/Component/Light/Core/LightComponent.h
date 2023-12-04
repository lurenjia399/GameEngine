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

	virtual void Tick(float DeltaTime) override;

	void SetLightIntensity(XMFLOAT3 InLightIntensity);
	void SetLightType(ELightType InLightType);
	void SetRotateFunction(std::function<XMFLOAT3(float)> InRotateFunction);

	XMFLOAT3 GetLightIntensity();
	ELightType GetLightType();


protected:
	
private:
	XMFLOAT3 LightIntensity;
	ELightType LightType;

	std::function<XMFLOAT3(float)> RotateFunction;
	

};

#endif // !CLightComponent
