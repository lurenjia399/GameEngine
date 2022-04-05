#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
	FWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init();
	virtual int PostInit();

	virtual void Tick();

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();
private:
	bool InitWindows(FWinMainCommandParameters InParameters);

	bool InitDirect3D();
protected:
	ComPtr<IDXGIFactory4>	DXGIFactory;	// 创建 DirectX 图形基础结构（DXGI）对象
	ComPtr<ID3D12Device>	D3dDevice;		// 创建 命令分配器，命令列表，命令队列，Fence, 资源，管道状态对象，堆
	ComPtr<ID3D12Fence>		Fence;

	ComPtr<ID3D12CommandQueue>			CommandQueue;
	ComPtr<ID3D12CommandAllocator>		CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	GraphicsCommamdList;

	ComPtr<IDXGISwapChain> SwapChain;

	//创建描述符和堆
	ComPtr<ID3D12DescriptorHeap> RTVHeap;//(running target view) 渲染目标纹理
	ComPtr<ID3D12DescriptorHeap> DSVHeap;//(depth stencil view) 渲染目标深度/模板共享纹理

protected:
	HWND MyWindowsHandle;		//主窗口句柄
	UINT M4XQualityLevels;		//多重采样的质量等级
	bool bMSAA4XEnabled;		//多重采样是否开启
	DXGI_FORMAT BufferFormat;	//格式纹理

};
#endif