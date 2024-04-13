#pragma once

#ifndef FFogConstantBuffer_H
#define FFogConstantBuffer_H

#include "../../../Core/Engine.h"

struct FFogConstantBuffer
{
	FFogConstantBuffer();

	XMFLOAT3 FogColor;
	float FogStart;

	float FogRange;
	float FogHeight;
	float FogTransparentCoefficient;
	float xx2;
};


#endif // !FFogConstantBuffer_H
