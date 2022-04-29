#include "DirectXDescriptorHeap.h"

ID3D12DescriptorHeap* FDirectXDescriptorHeap::GetHeap()
{
	return CBVHeap.Get();
}

void FDirectXDescriptorHeap::BuildDescriptorHeap(const UINT& InHeapCount)
{
	//创建常量缓冲区描述堆
	D3D12_DESCRIPTOR_HEAP_DESC CBVDescriptorHeapDesc = {};
	CBVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	CBVDescriptorHeapDesc.NumDescriptors = InHeapCount;
	CBVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(GetD3dDevice()->CreateDescriptorHeap(&CBVDescriptorHeapDesc, IID_PPV_ARGS(&CBVHeap)));
}
