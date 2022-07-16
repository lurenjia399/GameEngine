#pragma once

#ifndef FFogTransformation_H
#define FFogTransformation_H

#include "../../../Core/Engine.h"

struct FFogTransformation
{
	FFogTransformation();

	XMFLOAT3 FogColor;
	float FogStart;

	float FogRange;
	float FogHeight;
	float xx1;
	float xx2;
};


#endif // !FFogTransformation_H
