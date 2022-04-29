#include "DirectXConstBufferView.h"

void FDirectXConstBufferView::CreateConstant(UINT BufferSize, UINT BufferCount)
{
	ConstantBufferView = make_shared<FRenderingResourcesUpdate>();
	ConstantBufferView->Init(GetD3dDevice().Get(), BufferSize, BufferCount);

}

void FDirectXConstBufferView::BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE Handle, UINT ConstantBufferCount)
{
	D3D12_GPU_VIRTUAL_ADDRESS ConstantBufferAddress = ConstantBufferView.get()->GetBuffer()->GetGPUVirtualAddress();
	for (UINT i = 0; i < ConstantBufferCount; ++i)
	{
		UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		Handle.Offset(i, HandleSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC viewportDesc = {};
		//这里不确定，，需不需要
		viewportDesc.BufferLocation = ConstantBufferAddress + i * ConstantBufferView->GetConstantBufferByteSize();
		viewportDesc.SizeInBytes = ConstantBufferView->GetConstantBufferByteSize();

		GetD3dDevice()->CreateConstantBufferView(&viewportDesc, Handle);
	}
}
