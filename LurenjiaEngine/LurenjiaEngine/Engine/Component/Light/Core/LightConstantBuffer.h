#pragma once

#ifndef LightConstantBuffer_H
#define LightConstantBuffer_H
#include "../../../../Engine/Core/Engine.h"

struct Light
{
	XMFLOAT3 LightIntensity;
	int XX;

	XMFLOAT3 LightDirection;
	int XX2;
};



struct FLightConstantBuffer
{
	FLightConstantBuffer();

	Light SceneLight[16];
};

#endif // !LightConstantBuffer
