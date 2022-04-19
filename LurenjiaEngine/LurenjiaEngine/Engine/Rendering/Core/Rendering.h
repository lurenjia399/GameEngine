#pragma once
#include "../../Core/Engine.h"
#include "../../Platform/Windows/WindowsEngine.h"

class IRenderingInterface :virtual public IGuidInterface
{
	friend class CWindowsEngine;
public:
	IRenderingInterface();
	virtual ~IRenderingInterface() = 0;//纯虚析构函数需要写实现，并且此类无法实例化

	virtual void Init() = 0;

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime) = 0;
	virtual void PostDraw(float DeltaTime);

protected:
	ComPtr<ID3D12Resource> ConstructGPUDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSize);
protected:
	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();

#if defined(_WIN32)
	CWindowsEngine* GetEngine();
#else
	Engien* GetEngine();
#endif
	
private:
	static vector<IRenderingInterface*> RenderingInterface;
};

class FRenderingResourcesUpdate : public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();
	~FRenderingResourcesUpdate();

	void Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount);

	void Update(int Index, const void* InData);

	UINT GetConstantBufferByteSize(UINT InTypeSize);
	UINT GetConstantBufferByteSize();

	ID3D12Resource* GetBuffer() { return UploadBuffer.Get(); }
private:
	ComPtr<ID3D12Resource> UploadBuffer;
	UINT ElementSize;
	BYTE* Data;
};