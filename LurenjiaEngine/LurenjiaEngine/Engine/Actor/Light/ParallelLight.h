#pragma once

#ifndef ParallelLightH
#define ParallelLightH

#include "../Core/Actor.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Component/Light/Core/LightComponent.h"
#include "../../Component/Light/ParallelLightComponent.h"
#include "Core/Light.h"

class AParallelLight : public ALight, public IDirectXDeviceInterface
{
	typedef ALight Super;
public:
	AParallelLight();

	void SetRotateFunction(std::function<void(float)> RotateFunction);
	void FaceTarget(const XMFLOAT3& InPosition, const XMFLOAT3& InTargetPosition, const XMFLOAT3& InUpDirection = XMFLOAT3(0.f, 0.f, 1.f));
	
};


#endif