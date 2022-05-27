#pragma once

#ifndef LightConstantBuffer
#define LightConstantBuffer
#include "../../../../Engine/Core/Engine.h"


struct FLightConstantBuffer
{
	FLightConstantBuffer();

	XMFLOAT3 LightIntensity;
	int XX;

	XMFLOAT3 LightDirection;
	int XX2;
};

#endif // !LightConstantBuffer
