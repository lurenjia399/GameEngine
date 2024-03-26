#pragma once
#include "../../../Core/Engine.h"
#include "../../../Interface/DirectXDeviceInterface.h"

namespace ConstructBuffer
{
	struct FConstructBuffer : public IDirectXDeviceInterface_struct
	{
		ComPtr<ID3D12Resource> ConstructGPUDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSize);
	};
}