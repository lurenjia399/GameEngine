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
	void SetRotateFunction(std::function<void(float)> InRotateFunction);
	void FaceTarget(const XMFLOAT3& InPosition, const XMFLOAT3& InTargetPosition, const XMFLOAT3& InUpDirection = XMFLOAT3(0.f, 0.f, 1.f));

	XMFLOAT3 GetLightIntensity();
	ELightType GetLightType();


protected:
	
private:
	XMFLOAT3 LightIntensity;
	ELightType LightType;

	std::function<void(float)> RotateFunction;
	

};

#endif // !CLightComponent
