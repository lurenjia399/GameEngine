#pragma once
#include "../../Core/Engine.h"

class IRenderingInterface
{
	friend class FWindowsEngine;
public:
	IRenderingInterface();
	virtual ~IRenderingInterface() = 0;//��������������Ҫдʵ��

	virtual void Init() = 0;
	virtual void Draw(float DeltaTime) = 0;

	bool operator==(const IRenderingInterface& other)
	{
		return guid_equal(&this->Guid, &other.Guid);
	}

	simple_c_guid GetGuid() { return Guid; }
protected:
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT InDataSize);
protected:
	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
private:
	static vector<IRenderingInterface*> RenderingInterface;
	simple_c_guid Guid;
};

class FRenderingResourcesUpdate : public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();
	~FRenderingResourcesUpdate();

	void Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount);

	void Update(int Index, const void* InData);

	UINT GetConstantBufferByteSize(UINT InTypeSize);
	UINT GetConstantBufferByteSize() { return GetConstantBufferByteSize(ElementSize); }

	ID3D12Resource* GetBuffer() { return UploadBuffer.Get(); }
private:

	ComPtr<ID3D12Resource> UploadBuffer;
	UINT ElementSize;
	BYTE* Data;
};