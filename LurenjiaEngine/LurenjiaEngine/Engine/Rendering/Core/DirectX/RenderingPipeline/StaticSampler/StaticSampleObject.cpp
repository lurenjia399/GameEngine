#include "StaticSampleObject.h"

FStaticSampleObject::FStaticSampleObject()
{
}

void FStaticSampleObject::BuildStaticSampler()
{
	SamplerDesc.emplace_back(CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER::D3D12_FILTER_MIN_MAG_MIP_POINT));
	SamplerDesc.emplace_back(CD3DX12_STATIC_SAMPLER_DESC(1,
		D3D12_FILTER::D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		0.f, 8));
}																									

D3D12_STATIC_SAMPLER_DESC* FStaticSampleObject::GetData()
{
	return SamplerDesc.data();
}

int FStaticSampleObject::GetSize()
{
	return SamplerDesc.size();
}
