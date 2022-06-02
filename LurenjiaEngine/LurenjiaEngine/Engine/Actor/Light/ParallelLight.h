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
public:
	//获取组件变换相关
	XMFLOAT3 GetComponentPosition();
	fvector_3d GetComponentRotation();
	XMFLOAT3 GetComponentScale();
	void SetComponentPosition(const XMFLOAT3& InPosition);
	void SetComponentRotation(const fvector_3d& InRotation);
	void SetComponentScale(const XMFLOAT3& InScale);
private:
	CParallelLightComponent* ParallelLightComponent;
	CMeshComponent* ParallelLightMeshComponent;
};


#endif