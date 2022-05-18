#pragma once

#ifndef LightConstantBuffer
#define LightConstantBuffer
#include "../../../../Engine/Core/Engine.h"


struct FLightConstantBuffer
{
	FLightConstantBuffer();

	//XMFLOAT3 LightIntensity;
	XMFLOAT3 LightDirection;
};

#endif // !LightConstantBuffer
