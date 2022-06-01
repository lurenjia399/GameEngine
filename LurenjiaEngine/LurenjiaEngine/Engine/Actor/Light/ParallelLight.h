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

public:
	//��ȡ����任���
	void SetComponentPosition(const XMFLOAT3& InPosition);
	void SetComponentRotation(const fvector_3d& InRotation);
	void SetComponentScale(const XMFLOAT3& InScale);
private:
	CParallelLightComponent* ParallelLightComponent;
	CMeshComponent* ParallelLightMeshComponent;
};


#endif