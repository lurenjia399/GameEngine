#pragma once
#include "../Core/Engine.h"

#if defined(_WIN32)
class CWindowsEngine;
class CMeshManage;
class CLightManage;
class CRenderingEngine;
#else
class CEngien;
#endif

class IDirectXDeviceInterface
{
public:
	void StartSetMainViewportRenderTarget();// 开始设置主视口的rendertarget
	void EndSetMainViewportRenderTarget();// 结束设置主视口的rendertarget
	void ClearMainSwapChain();// 清除主视口的交换链
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
	//获取mesh管理器
	CMeshManage* GetMeshManage() const;
	//获取light管理器
	CLightManage* GetLightManage() const;
	//获取RTV描述符堆
	ComPtr<ID3D12DescriptorHeap> GetRTVHeap() const;
	//获取DSV描述符堆
	ComPtr<ID3D12DescriptorHeap> GetDSVHeap() const;
	//获取某种描述符大小
	UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE InDescriptorHeapType) const;
	//获取引擎
#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	CEngien* GetEngine() const;
#endif
	//获取渲染引擎
	CRenderingEngine* GetRenderEngine() const;
	
};

struct IDirectXDeviceInterface_struct
{
	// 开始设置主视口的rendertarget
	void StartSetMainViewportRenderTarget();
	// 结束设置主视口的rendertarget
	void EndSetMainViewportRenderTarget();
	// 清除主视口的交换链
	void ClearMainSwapChain();


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
	//获取mesh管理器
	CMeshManage* GetMeshManage() const;
	//获取light管理器
	CLightManage* GetLightManage() const;
	//获取RTV描述符堆
	ComPtr<ID3D12DescriptorHeap> GetRTVHeap() const;
	//获取DSV描述符堆
	ComPtr<ID3D12DescriptorHeap> GetDSVHeap() const;
	//获取某种描述符大小
	UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE InDescriptorHeapType) const;
#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	Engien* GetEngine() const;
#endif
	
private:
	IDirectXDeviceInterface DXDeviceInterface;
};
