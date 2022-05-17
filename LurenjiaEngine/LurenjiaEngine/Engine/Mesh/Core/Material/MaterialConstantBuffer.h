#pragma once

#ifndef MaterialConstantBuffer
#define MaterialConstantBuffer

#include "../../../LurenjiaEngine.h"

struct CMaterialConstantBuffer
{
public:
	CMaterialConstantBuffer();

	XMFLOAT4 BaseColor;
	XMFLOAT4X4 TransformInformation;
};
#endif // !MaterialConstantBuffer
