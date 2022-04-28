#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"


struct FDirectXRootSignature : public IDirectXDeviceInterface_struct
{
public:
	ComPtr<ID3D12RootSignature> RootSignature;
};