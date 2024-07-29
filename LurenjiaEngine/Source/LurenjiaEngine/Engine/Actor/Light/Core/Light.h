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
	void Tick(float DeltaTime) override;
	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;

	void SetLightIntensity(const XMFLOAT3& InLightIntensity);

	template<typename T>
	T* GetMeshComponet()
	{
		if (LightComponent)
		{
			return LightComponent->GetLightMeshComponent();
		}
		return nullptr;
	}
public:
	XMFLOAT3 GetPosition() const override;
	fvector_3d GetRotation() const override;
	XMFLOAT3 GetScale() const override;
	XMFLOAT3 GetForward() const override;
	XMFLOAT3 GetRight() const override;
	XMFLOAT3 GetUp() const override;

	XMFLOAT3 GetLightIntensity();
protected:
	CLightComponent* LightComponent;
};

#endif // !Light_H
