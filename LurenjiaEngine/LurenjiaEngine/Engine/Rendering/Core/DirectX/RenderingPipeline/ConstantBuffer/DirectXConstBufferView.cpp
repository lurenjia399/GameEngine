#include "DirectXConstBufferView.h"

void FDirectXConstBufferView::CreateConstant(UINT BufferSize, UINT BufferCount, bool bIsConstantBuffer)
{
	ConstantBufferView = make_shared<FRenderingResourcesUpdate>();
	ConstantBufferView->Init(GetD3dDevice().Get(), BufferSize, BufferCount, bIsConstantBuffer);

}

void FDirectXConstBufferView::BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle, UINT ConstantBufferCount, UINT HandleOffset)
{
	D3D12_GPU_VIRTUAL_ADDRESS ConstantBufferAddress = ConstantBufferView->GetBuffer()->GetGPUVirtualAddress();
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	for (UINT i = 0; i < ConstantBufferCount; ++i)
	{
		//每一帧都是起始地址
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = InHandle;
		D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
		CBVDesc.BufferLocation = ConstantBufferAddress + i * ConstantBufferView->GetConstantBufferByteSize();
		CBVDesc.SizeInBytes = ConstantBufferView->GetConstantBufferByteSize();
		Handle.Offset(i + HandleOffset, HandleSize);
		GetD3dDevice()->CreateConstantBufferView(&CBVDesc, Handle);
	}
}

void FDirectXConstBufferView::Update(int Index, const void* InData)
{
	ConstantBufferView->Update(Index, InData);
}

ID3D12Resource* FDirectXConstBufferView::GetBuffer()
{
	return ConstantBufferView->GetBuffer();
}
