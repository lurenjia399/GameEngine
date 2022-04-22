#include "Rendering.h"
#include "../Engine/DirectX/Core/DirectXRenderingEngine.h"

//vector<IRenderingInterface*> IRenderingInterface::RenderingInterface = vector<IRenderingInterface*>();

IRenderingInterface::IRenderingInterface()
{
	//RenderingInterface.emplace_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	/*for (vector<IRenderingInterface*>::const_iterator iter = RenderingInterface.begin(); iter != RenderingInterface.end(); iter++)
	{
		if (*iter == this)
		{
			RenderingInterface.erase(iter);
			break;
		}
	}*/
}

void IRenderingInterface::PreDraw(float DeltaTime)
{
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), nullptr);
}

void IRenderingInterface::PostDraw(float DeltaTime)
{
}

ComPtr<ID3D12Resource> IRenderingInterface::ConstructGPUDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSizeInBytes)
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer D3dDevice is nullptr");
		return nullptr;
	}

	ComPtr<ID3D12Resource> Buffer;//���ؽ��
	D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSizeInBytes);
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//��Buffer��Դ�ύ��defaultHeap��
	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(Buffer.GetAddressOf())));

	//��OutTempBuffer��Դ�ύ��UploadHeap��
	D3D12_HEAP_PROPERTIES UpdataBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&UpdataBufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//generic ͨ�õ�
		nullptr,
		IID_PPV_ARGS(OutTempBuffer.GetAddressOf())));

	D3D12_SUBRESOURCE_DATA SubRresourceData = {};
	SubRresourceData.pData = InData;						//subResource ������
	SubRresourceData.RowPitch = InDataSizeInBytes;			//subResource �����ݴ�С
	SubRresourceData.SlicePitch = SubRresourceData.RowPitch;//subResource ����ȼ��

	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
	if (GraphicsCommandList == nullptr)
	{
		Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer GraphicsCommandList is nullptr");
		return nullptr;
	}
	//�ı�Buffer��Դ��״̬
	D3D12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	GraphicsCommandList->ResourceBarrier(1, &CopyDestBarrier);
	
	//�����ݷŵ�Buffer��
	UpdateSubresources<1>(
		GraphicsCommandList.Get(),	//�����б�
		Buffer.Get(),				//Ŀ�껺����
		OutTempBuffer.Get(),		//�м仺�����������ϴ����У�
		0,							//�м仺�����е���Դƫ��
		0,							//��Դ�е�һ������Դ����
		1,							//��Դ������Դ������
		&SubRresourceData);			//����Դ

	//�ı�Buffer��Դ��״̬
	D3D12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	GraphicsCommandList->ResourceBarrier(1, &ReadDestBarrier);

	return Buffer;
}

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{
	CWindowsEngine* WindowsEngine = GetEngine();
	if (WindowsEngine && WindowsEngine->GetRenderingEngine())
	{
		return WindowsEngine->GetRenderingEngine()->GetD3dDevice();
	}
	return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetGraphicsCommandList()
{
	CWindowsEngine* WindowsEngine = GetEngine();
	if (WindowsEngine && WindowsEngine->GetRenderingEngine())
	{
		return WindowsEngine->GetRenderingEngine()->GetGraphicsCommandList();
	}
	return nullptr;
}
ComPtr<ID3D12CommandAllocator> IRenderingInterface::GetCommandAllocator()
{
	CWindowsEngine* WindowsEngine = GetEngine();
	if (WindowsEngine && WindowsEngine->GetRenderingEngine())
	{
		return WindowsEngine->GetRenderingEngine()->GetCommandAllocator();
	}
	return nullptr;
}
#if defined(_WIN32)
CWindowsEngine* IRenderingInterface::GetEngine()
{
	if (Engine != nullptr)
	{
		CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
		if (WindowsEngine != nullptr)
		{
			return WindowsEngine;
		}
		return WindowsEngine;
	}
	return nullptr;
}
#else
Engine* IRenderingInterface::GetEngine()
{
	return Engine != nullptr ? Engine : nullptr;
}
#endif



FRenderingResourcesUpdate::FRenderingResourcesUpdate()
	: UploadBuffer(nullptr)
	, ElementSize(0)
	, Data(nullptr)
{
}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer != nullptr)
	{
		UploadBuffer->Unmap(0, nullptr);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount)
{
	assert(InDevice);

	ElementSize = InElemetSize;

	D3D12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InElemetSize * InElemetCount);//GetConstantBufferByteSize()InElemetSize * InElemetCount
	ANALYSIS_HRESULT(InDevice->CreateCommittedResource(
		&HeapPropertie,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&UploadBuffer)));
	//ӳ�䣬����Դӳ�䵽�Դ���
	ANALYSIS_HRESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	memcpy(&Data[Index * ElementSize], InData, ElementSize);
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT InTypeSize)
{
	//��߹涨ȡ256��������
	return (InTypeSize + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(ElementSize);
}
