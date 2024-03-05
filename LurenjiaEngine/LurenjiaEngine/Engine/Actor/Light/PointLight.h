#pragma once

#ifndef PointLightLight_H
#define PointLightLight_H

#include "Core/RangeLight.h"
#include "../../Interface/DirectXDeviceInterface.h"

class APointLight : public ARangeLight, public IDirectXDeviceInterface
{
	typedef ARangeLight Super;
public:
	APointLight();

	void Tick(float DeltaTime) override;
private:
	float time;
};


#endif