#pragma once

#ifndef SpotLightLight_H
#define SpotLightLight_H

#include "../Core/Actor.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Component/Light/SpotLightComponent.h"
#include "../../Component/Mesh/Core/MeshComponent.h"

class ASpotLight : public AActor, public IDirectXDeviceInterface
{
public:
	ASpotLight();

	void Tick(float DeltaTime) override;
	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;

	void SetLightIntensity(const XMFLOAT3& InLightIntensity);
	void SetLightStartAttenuation(const float& InStartAttenuation);
	void SetLightEndAttenuation(const float& InEndAttenuation);
public:
	//获取组件变换相关
	XMFLOAT3 GetPosition() const override;
	fvector_3d GetRotation() const override;
	XMFLOAT3 GetScale() const override;
	XMFLOAT3 GetForward() const override;
	XMFLOAT3 GetRight() const override;
	XMFLOAT3 GetUp() const override;

	XMFLOAT3 GetLightIntensity();
	float GetLightStartAttenuation();
	float GetLightEndAttenuation();
	
private:
	CSpotLightComponent* SpotLightComponent;
};


#endif