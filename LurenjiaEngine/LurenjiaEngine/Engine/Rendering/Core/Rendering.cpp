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
	ComPtr<ID3D12Resource> Buffer;//���ؽ��
	D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{ 
		Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer D3dDevice is nullptr");
		return nullptr;
	}
	//���Ƚ����ؽ���ύ������
	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(Buffer.GetAddressOf())));

	//��OutTempBuffer����Ĭ�϶ѣ���Դ��״̬�ǿɶ���
	D3D12_HEAP_PROPERTIES UpdataBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&UpdataBufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//generic ͨ�õ�
		nullptr,
		IID_PPV_ARGS(OutTempBuffer.GetAddressOf())));

	D3D12_SUBRESOURCE_DATA SubRresourceData = {};
	SubRresourceData.pData = InData;						//subResource ������
	SubRresourceData.RowPitch = InDataSize;					//subResource ���м��
	SubRresourceData.SlicePitch = SubRresourceData.RowPitch;//subResource ����ȼ��

	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
	if (GraphicsCommandList == nullptr)
	{
		Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer GraphicsCommandList is nullptr");
		return nullptr;
	}
	//����Buffer����Դ����
	D3D12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	GraphicsCommandList->ResourceBarrier(1, &CopyDestBarrier);
	
	UpdateSubresources<1>(
		GraphicsCommandList.Get(),	//�����б�
		Buffer.Get(),				//Ŀ����Դ
		OutTempBuffer.Get(),		//�м���Դ
		0,							//�м���Դ��λ��ƫ��
		0,							//��Դ�е�һ������Դ����
		1,							//����Դ������
		&SubRresourceData);			//��Դ��Դ

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

FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{
}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer)
	{
		UploadBuffer->Unmap(0, nullptr);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount)
{
	assert(InDevice);

	ElementSize = InElemetSize;

	D3D12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InElemetSize * InElemetCount);
	InDevice->CreateCommittedResource(
		&HeapPropertie,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(UploadBuffer.GetAddressOf()));
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
