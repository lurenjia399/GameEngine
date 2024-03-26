#pragma once

#ifndef StaticSampleObject_H
#define StaticSampleObject_H

#include "../../../../../Interface/DirectXDeviceInterface.h"

struct FStaticSampleObject : public IDirectXDeviceInterface_struct
{
public:
	FStaticSampleObject();

	void BuildStaticSampler();

	D3D12_STATIC_SAMPLER_DESC* GetData();
	int GetSize();
private:

	//静态采样方式
	std::vector<D3D12_STATIC_SAMPLER_DESC> SamplerDesc;
};

#endif