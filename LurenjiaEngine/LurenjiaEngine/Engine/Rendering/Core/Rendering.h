#pragma once
#include "../../Core/Engine.h"

class IRenderingInterface
{
	friend class FWindowsEngine;
public:
	IRenderingInterface();
	virtual ~IRenderingInterface() = 0;//纯虚析构函数需要写实现

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