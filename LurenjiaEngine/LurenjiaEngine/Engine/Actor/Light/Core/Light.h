#pragma once

#ifndef Light_H
#define Light_H

#include "../../Core/Actor.h"
#include "../../../Component/Light/Core/LightComponent.h"

class ALight : public AActor
{
public:
	ALight();
public:
	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;
public:
	XMFLOAT3 GetPosition() const override;
	fvector_3d GetRotation() const override;
	XMFLOAT3 GetScale() const override;
	XMFLOAT3 GetForward() const override;
	XMFLOAT3 GetRight() const override;
	XMFLOAT3 GetUp() const override;
protected:
	CLightComponent* LightComponent;
};

#endif // !Light_H
