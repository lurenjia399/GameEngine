#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"


struct FDirectXRootSignature : public IDirectXDeviceInterface_struct
{
public:
	FDirectXRootSignature();
	bool Build();
	ID3D12RootSignature* GetRootSignature();
private:
	ComPtr<ID3D12RootSignature> RootSignature;
};