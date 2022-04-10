#include "Rendering.h"
#include "../../Platform/Windows/WindowsEngine.h"

vector<IRenderingInterface*> IRenderingInterface::RenderingInterface = vector<IRenderingInterface*>();

IRenderingInterface::IRenderingInterface()
{
	create_guid(&Guid);
	RenderingInterface.emplace_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	for (vector<IRenderingInterface*>::const_iterator iter = RenderingInterface.begin(); iter != RenderingInterface.end(); iter++)
	{
		if (*iter == this)
		{
			RenderingInterface.erase(iter);
			break;
		}
	}
}

ComPtr<ID3D12Resource> IRenderingInterface::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT InDataSize)
{
	ComPtr<ID3D12Resource> Buffer;
	D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{ 
		Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer D3dDevice is nullptr");
		return nullptr;
	}
	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(Buffer.GetAddressOf())));

	//将OutTempBuffer放入可读堆中
	D3D12_HEAP_PROPERTIES UpdataBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&UpdataBufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//generic 通用的
		nullptr,
		IID_PPV_ARGS(OutTempBuffer.GetAddressOf())));

	D3D12_SUBRESOURCE_DATA SubRresourceData = {};
	SubRresourceData.pData = InData;						//subResource 的数据
	SubRresourceData.RowPitch = InDataSize;					//subResource 的行间距
	SubRresourceData.SlicePitch = SubRresourceData.RowPitch;//subResource 的深度间距

	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
	if (GraphicsCommandList == nullptr)
	{
		Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer GraphicsCommandList is nullptr");
		return nullptr;
	}
	//设置Buffer的资源屏障
	D3D12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	GraphicsCommandList->ResourceBarrier(1, &CopyDestBarrier);
	
	UpdateSubresources<1>(
		GraphicsCommandList.Get(),
		Buffer.Get(),
		OutTempBuffer.Get(),
		0, 0, 1, &SubRresourceData);

	D3D12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	GraphicsCommandList->ResourceBarrier(1, &ReadDestBarrier);
	//GetGraphicsCommandList()->Close();

	return Buffer;
}

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{

	if (Engine)
	{
		FWindowsEngine* WindowsEngine = dynamic_cast<FWindowsEngine*>(Engine);
		if (WindowsEngine)
		{
			return WindowsEngine->D3dDevice;
		}
		return nullptr;
	}
	return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetGraphicsCommandList()
{
	if (Engine)
	{
		FWindowsEngine* WindowsEngine = dynamic_cast<FWindowsEngine*>(Engine);
		if (WindowsEngine)
		{
			return WindowsEngine->GraphicsCommandList;
		}
		return nullptr;
	}
	return nullptr;
}
