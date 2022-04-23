#pragma once
#include "../Core/Engine.h"

#if defined(_WIN32)
class CWindowsEngine;
#else
class CEngien;
#endif

class IDirectXDeviceInterface
{
public:
	//获取当前围栏
	ComPtr<ID3D12Fence> GetFence() const;
	//获取引擎驱动
	ComPtr<ID3D12Device> GetD3dDevice() const;
	//获取命令列表
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	//获取命令分配器
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	//获取命令队列
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
	//获取windowsHandle
	HWND GetMainWindowsHandle() const;

#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	CEngien* GetEngine() const;
#endif
};

struct IDirectXDeviceInterface_struct
{
	//获取当前围栏
	ComPtr<ID3D12Fence> GetFence() const;
	//获取引擎驱动
	ComPtr<ID3D12Device> GetD3dDevice() const;
	//获取命令列表
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	//获取命令分配器
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	//获取命令队列
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
	//获取windowsHandle
	HWND GetMainWindowsHandle() const;

#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	Engien* GetEngine() const;
#endif
private:
	IDirectXDeviceInterface DXDeviceInterface;
};
