#pragma once

#ifndef LightConstantBuffer_H
#define LightConstantBuffer_H
#include "../../../../Engine/Core/Engine.h"

struct FLight
{
	XMFLOAT3	LightIntensity;			//光源强度
	float		StartAttenuation;		//点光源开始衰减距离（离点光源中心位置的距离）

	XMFLOAT3	LightDirection;			//光源方向
	float		EndAttenuation;			//点光源的结束衰减（离点光源中心位置的距离）

	XMFLOAT3	Position;               //灯光位置
	int			LightType;              //灯光类型
};



struct FLightConstantBuffer
{
	FLightConstantBuffer();

	FLight SceneLight[16];
};

#endif // !LightConstantBuffer
