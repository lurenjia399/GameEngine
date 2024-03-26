#pragma once

#ifndef RangeLight_H
#define RangeLight_H

#include "../../Core/Actor.h"
#include "../../../Component/Light/Core/RangeLightComponent.h"
#include "Light.h"

class ARangeLight : public ALight
{
public:
	ARangeLight();
public:
	void SetLightStartAttenuation(const float& InStartAttenuation);
	void SetLightEndAttenuation(const float& InEndAttenuation);
public:
	float GetLightStartAttenuation();
	float GetLightEndAttenuation();
};

#endif // !Light_H
