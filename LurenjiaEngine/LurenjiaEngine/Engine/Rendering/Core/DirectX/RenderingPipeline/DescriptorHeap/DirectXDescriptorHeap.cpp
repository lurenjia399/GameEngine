#include "DirectXDescriptorHeap.h"

FDirectXDescriptorHeap::FDirectXDescriptorHeap()
	: RenderingDescriptorHeap(ComPtr<ID3D12DescriptorHeap>())
{
}

ID3D12DescriptorHeap* FDirectXDescriptorHeap::GetRenderingHeap()
{
	return RenderingDescriptorHeap.Get();
}

void FDirectXDescriptorHeap::BuildRenderingDescriptorHeap(const UINT& InHeapCount)
{
	D3D12_DESCRIPTOR_HEAP_DESC CBVDescriptorHeapDesc = {};
	CBVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	CBVDescriptorHeapDesc.NumDescriptors = InHeapCount;
	CBVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(GetD3dDevice()->CreateDescriptorHeap(&CBVDescriptorHeapDesc, IID_PPV_ARGS(&RenderingDescriptorHeap)));
}


void FDirectXDescriptorHeap::PreDraw(float DeltaTime)
{
	ID3D12DescriptorHeap* DescriptorHeap[] = { RenderingDescriptorHeap.Get() };
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
	Engine_Log("ComputeDescriptorHeap and RenderingDescriptorHeap create")
	
}
