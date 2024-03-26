#include "ConstructBuffer.h"
#include "../../../Interface/DirectXDeviceInterface.h"


namespace ConstructBuffer
{
	ComPtr<ID3D12Resource> FConstructBuffer::ConstructGPUDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSize)
	{
		ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
		if (D3dDevice == nullptr)
		{
			Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer D3dDevice is nullptr");
			return nullptr;
		}

		ComPtr<ID3D12Resource> Buffer;//返回结果
		D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
		D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		//将Buffer资源提交到defaultHeap中
		ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
			&HeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(Buffer.GetAddressOf())));

		//将OutTempBuffer资源提交到UploadHeap中
		D3D12_HEAP_PROPERTIES UpdataBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
			&UpdataBufferProperties,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,//generic 通用的
			nullptr,
			IID_PPV_ARGS(OutTempBuffer.GetAddressOf())));

		D3D12_SUBRESOURCE_DATA SubRresourceData = {};
		SubRresourceData.pData = InData;						//subResource 的数据
		SubRresourceData.RowPitch = InDataSize;					//subResource 的数据大小
		SubRresourceData.SlicePitch = SubRresourceData.RowPitch;//subResource 的深度间距

		ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
		if (GraphicsCommandList == nullptr)
		{
			Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer GraphicsCommandList is nullptr");
			return nullptr;
		}
		//改变Buffer资源的状态
		D3D12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		GraphicsCommandList->ResourceBarrier(1, &CopyDestBarrier);

		//将数据放到Buffer中
		UpdateSubresources<1>(
			GraphicsCommandList.Get(),	//命令列表
			Buffer.Get(),				//目标缓冲区
			OutTempBuffer.Get(),		//中间缓冲区（放在上传堆中）
			0,							//中间缓冲区中的资源偏移
			0,							//资源中第一个子资源索引
			1,							//资源中子资源的数量
			&SubRresourceData);			//子资源

		//改变Buffer资源的状态
		D3D12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		GraphicsCommandList->ResourceBarrier(1, &ReadDestBarrier);

		return Buffer;
	}
}