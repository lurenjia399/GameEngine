#pragma once

#ifndef MaterialConstantBuffer_H
#define MaterialConstantBuffer_H

#include "../../../LurenjiaEngine.h"
#include "MaterialType.h"

struct CMaterialConstantBuffer
{
public:
	CMaterialConstantBuffer();
	
	XMFLOAT4 BaseColor;

	UINT32 MaterialType;
	float Roughness;
	int TextureMapIndex; //使用的贴图序号，不使用为-1
	int NormalMapIndex;//使用法线贴图序号， 不使用为-1

	int SpecularMapIndex;//使用的高光贴图序号, 不适用为-1
	XMFLOAT3 SpecularColor;//默认使用的高光颜色基数

	XMFLOAT3 FresnelF0;
	float xx;

	XMFLOAT4X4 TransformInformation;
};
#endif // !MaterialConstantBuffer
