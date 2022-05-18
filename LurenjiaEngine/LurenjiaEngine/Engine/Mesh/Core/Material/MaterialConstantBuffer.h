#pragma once

#ifndef MaterialConstantBufferH
#define MaterialConstantBufferH

#include "../../../LurenjiaEngine.h"
#include "MaterialType.h"

struct CMaterialConstantBuffer
{
public:
	CMaterialConstantBuffer();
	
	XMFLOAT4 BaseColor;
	UINT32 MaterialType;
	float Roughness;
	XMFLOAT4X4 TransformInformation;
};
#endif // !MaterialConstantBuffer
