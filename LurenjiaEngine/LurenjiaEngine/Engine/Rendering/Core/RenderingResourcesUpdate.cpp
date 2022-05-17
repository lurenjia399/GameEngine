#include "RenderingResourcesUpdate.h"

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

	ElementSize = GetConstantBufferByteSize(InElemetSize);

	D3D12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ElementSize * InElemetCount);//GetConstantBufferByteSize()InElemetSize * InElemetCount
	ANALYSIS_HRESULT(InDevice->CreateCommittedResource(
		&HeapPropertie,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&UploadBuffer)));
	//映射，将资源映射到显存中
	ANALYSIS_HRESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	memcpy(&Data[Index * ElementSize], InData, ElementSize);
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT InTypeSize)
{
	//上取整
	// (A + B - 1) / A
	//这边规定取256的整数倍
	return (InTypeSize + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(ElementSize);
}