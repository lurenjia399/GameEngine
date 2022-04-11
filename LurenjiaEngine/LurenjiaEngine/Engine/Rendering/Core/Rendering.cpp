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
	ComPtr<ID3D12Resource> Buffer;//返回结果
	D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{ 
		Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer D3dDevice is nullptr");
		return nullptr;
	}
	//首先将返回结果提交到堆中
	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(Buffer.GetAddressOf())));

	//将OutTempBuffer放入默认堆，资源的状态是可读的
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
		GraphicsCommandList.Get(),	//命令列表
		Buffer.Get(),				//目标资源
		OutTempBuffer.Get(),		//中间资源
		0,							//中间资源的位置偏移
		0,							//资源中第一个子资源索引
		1,							//子资源的数量
		&SubRresourceData);			//资源来源

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
