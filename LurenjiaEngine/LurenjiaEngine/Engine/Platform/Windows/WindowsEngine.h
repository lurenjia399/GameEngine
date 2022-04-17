#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
	FWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters& InParameters);
	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	virtual void Tick(float DeltaTime);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();
//----------interface-----
public:
	//获取引擎驱动
	ComPtr<ID3D12Device> GetD3dDevice() const;
	//获取命令列表
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	//获取命令分配器
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	//获取交换链中的buffer
	ID3D12Resource* GetCurrentSwapBuffer() const;
	//获取当前的资源描述符
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;
	//获取当前的深度模板描述符
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDSBufferView() const;
	//获取当前的后缓冲描述符
	DXGI_FORMAT GetBackBufferFormat() const;
	//获取当前的深度模板描述符
	DXGI_FORMAT GetDepthStencilFormat() const;
	//获取当前采样数量
	UINT GetDXGISampleCount() const;
	//获取当前采样质量
	UINT GetDXGISampleQuality() const;
protected:
	//等待gpu处理完名列队列中的数据
	void WaitGPUCommandQueueComplete();
//----------interface-----
private:
	bool InitWindows(FWinMainCommandParameters InParameters);
	bool InitDirect3D();
	bool PostInitDirect3D();
protected:

	UINT CurrentFenceIndex;					//当前的围栏值
	int CurrentSwapBufferIndex;				//当前交换链中所使用的缓冲区index

	ComPtr<IDXGIFactory4>	DXGIFactory;	// 创建 DirectX 图形基础结构（DXGI）对象
	ComPtr<ID3D12Device>	D3dDevice;		// 创建 命令分配器，命令列表，命令队列，Fence, 资源，管道状态对象，堆
	ComPtr<ID3D12Fence>		Fence;

	ComPtr<ID3D12CommandQueue>			CommandQueue;
	ComPtr<ID3D12CommandAllocator>		CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	GraphicsCommandList;

	ComPtr<IDXGISwapChain> SwapChain;

	//创建描述符堆
	ComPtr<ID3D12DescriptorHeap> RTVHeap;//(running target view) 渲染目标纹理
	ComPtr<ID3D12DescriptorHeap> DSVHeap;//(depth stencil view) 渲染目标深度/模板共享纹理

	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;//交换链资源
	ComPtr<ID3D12Resource> DepthStencilBuffer;			//深度模板资源

	D3D12_VIEWPORT ViewPortInfo;
	D3D12_RECT ViewPortRect;
protected:
	HWND MyWindowsHandle;		//主窗口句柄
	UINT M4XQualityLevels;		//多重采样的质量等级
	bool bMSAA4XEnabled;		//多重采样是否开启
	DXGI_FORMAT BackBufferFormat;	//后台缓冲区格式
	DXGI_FORMAT DepthStencilFormat;	//深度模板格式
	UINT RTVDescriptorSize;		//rtv描述符大小
};
#endif