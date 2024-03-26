#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"


struct FDirectXDescriptorHeap : public IDirectXDeviceInterface_struct
{
public:
	FDirectXDescriptorHeap();

	ID3D12DescriptorHeap* GetRenderingHeap();
	void BuildRenderingDescriptorHeap(const UINT& InHeapCount);

	void PreDraw(float DeltaTime);
private:
	ComPtr<ID3D12DescriptorHeap> RenderingDescriptorHeap;				//‰÷»æ√Ë ˆ∑˚∂—
};