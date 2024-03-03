#pragma once

#ifndef SpotLightLight_H
#define SpotLightLight_H

#include "Core/RangeLight.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Component/Light/SpotLightComponent.h"

class ASpotLight : public ARangeLight, public IDirectXDeviceInterface
{
	typedef ARangeLight Super;
public:
	ASpotLight();
	virtual void BeginInit() override;

	void Tick(float DeltaTime) override;
public:
	void SetConicalInnerCorner(float InConicalInnerCorner);
	void SetConicalOuterCorner(float InConicalOuterCorner);

public:
	float GetConicalInnerCorner() const;
	float GetConicalOuterCorner() const;
};


#endif