#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"


struct FDirectXDescriptorHeap : public IDirectXDeviceInterface_struct
{
public:
	ID3D12DescriptorHeap* GetHeap();
	void BuildDescriptorHeap(const UINT& InHeapCount);
private:
	ComPtr<ID3D12DescriptorHeap> CBVHeap;					//≥£¡ø√Ë ˆ∂—
};