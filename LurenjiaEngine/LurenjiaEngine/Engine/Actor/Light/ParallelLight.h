#pragma once

#ifndef ParallelLightH
#define ParallelLightH

#include "../Core/Actor.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Component/Light/Core/LightComponent.h"
#include "../../Component/Light/ParallelLightComponent.h"

class AParallelLight : public AActor, public IDirectXDeviceInterface
{
public:
	AParallelLight();

	void Tick(float DeltaTime) override;
	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;

	void SetRotateFunction(std::function<void(float)> RotateFunction);
	void SetLightIntensity(const XMFLOAT3& InLightIntensity);
	void FaceTarget(const XMFLOAT3& InPosition, const XMFLOAT3& InTargetPosition, const XMFLOAT3& InUpDirection = XMFLOAT3(0.f, 0.f, 1.f));
public:
	//获取组件变换相关
	XMFLOAT3 GetPosition() const override;
	fvector_3d GetRotation() const override;
	XMFLOAT3 GetScale() const override;


	XMFLOAT3 GetLightIntensity();
	
private:
	CParallelLightComponent* ParallelLightComponent;
	
};


#endif