#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../StaticSampler/StaticSampleObject.h"


struct FDirectXRootSignature : public IDirectXDeviceInterface_struct
{
public:
	FDirectXRootSignature();
	bool Build(const UINT& TextureCount = 1);
	ID3D12RootSignature* GetRootSignature();
	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);
private:
	FStaticSampleObject StaticSamplerObject;
	ComPtr<ID3D12RootSignature> RootSignature;
};