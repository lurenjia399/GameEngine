#pragma once

#ifndef RangeLight_H
#define RangeLight_H

#include "../../Core/Actor.h"
#include "../../../Component/Light/Core/RangeLightComponent.h"

class ARangeLight : public AActor
{
public:
	ARangeLight();
public:
	void Tick(float DeltaTime) override;
	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;

	void SetLightIntensity(const XMFLOAT3& InLightIntensity);
	void SetLightStartAttenuation(const float& InStartAttenuation);
	void SetLightEndAttenuation(const float& InEndAttenuation);
public:
	XMFLOAT3 GetPosition() const override;
	fvector_3d GetRotation() const override;
	XMFLOAT3 GetScale() const override;
	XMFLOAT3 GetForward() const override;
	XMFLOAT3 GetRight() const override;
	XMFLOAT3 GetUp() const override;

	XMFLOAT3 GetLightIntensity();
	float GetLightStartAttenuation();
	float GetLightEndAttenuation();
protected:
	CRangeLightComponent* RangeLightComponent;
};

#endif // !Light_H
