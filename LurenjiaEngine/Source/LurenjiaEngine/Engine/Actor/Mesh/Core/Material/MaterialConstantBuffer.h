#pragma once

#ifndef MaterialConstantBuffer_H
#define MaterialConstantBuffer_H

#include "../../../../LurenjiaEngine.h"
#include "MaterialType.h"

struct CMaterialConstantBuffer
{
public:
	CMaterialConstantBuffer();
	
	XMFLOAT4 BaseColor;

	UINT32 MaterialType;
	float Roughness;
	int TextureMapIndex;			//texture index ,unused -1
	int NormalMapIndex;				//normal texture index ,unused -1

	int SpecularMapIndex;			//Specular texture index, unused -1
	XMFLOAT3 SpecularColor;			//default specular color pivot

	XMFLOAT3 FresnelF0;
	float xx;

	XMFLOAT4X4 TransformInformation;
};
#endif
