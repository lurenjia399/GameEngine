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

	void SetLightIntensity(const XMFLOAT3& InLightIntensity);
public:
	//获取组件变换相关
	XMFLOAT3 GetComponentPosition();
	fvector_3d GetComponentRotation();
	XMFLOAT3 GetComponentScale();

	XMFLOAT3 GetLightIntensity();
	
private:
	CParallelLightComponent* ParallelLightComponent;
	CMeshComponent* ParallelLightMeshComponent;
};


#endif