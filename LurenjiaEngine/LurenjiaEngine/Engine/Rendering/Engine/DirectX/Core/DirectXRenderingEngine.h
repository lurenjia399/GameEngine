#pragma
#include "../../Core/RenderingEngine.h"
#include "../../../../Mesh/Core/MeshManage.h"
#include "../../../../Core/Viewport/ViewportInfo.h"

class CDirectXRenderingEngine :public CRenderingEngine
{
	friend class IDirectXDeviceInterface;
public:
	CDirectXRenderingEngine();
	virtual int PreInit(FWinMainCommandParameters& InParameters) override;//重写父类的纯虚函数
	virtual int Init(FWinMainCommandParameters InParameters) override;//重写父类的纯虚函数
	virtual int PostInit() override;//重写父类的纯虚函数
	virtual void Tick(float DeltaTime) override;//重写父类的纯虚函数
	virtual int PreExit() override;//重写父类的纯虚函数
	virtual int Exit() override;//重写父类的纯虚函数
	virtual int PostExit() override;//重写父类的纯虚函数
	virtual void UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo) override;//重写父类的虚函数

public:
	//获取引擎驱动
	ComPtr<ID3D12Device> GetD3dDevice() const;
	//获取命令列表
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	//获取命令分配器
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	//获取当前的后缓冲描述符
	DXGI_FORMAT GetBackBufferFormat() const;
	//获取当前的深度模板描述符
	DXGI_FORMAT GetDepthStencilFormat() const;
	//获取当前的深度模板描述符
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDSBufferView() const;
	//获取当前的资源描述符
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;
	//获取交换链中的buffer
	ID3D12Resource* GetCurrentSwapBuffer() const;
	//获取当前采样数量
	UINT GetDXGISampleCount() const;
	//获取当前采样质量
	UINT GetDXGISampleQuality() const;
private:
	//等待gpu处理完名列队列中的数据
	void WaitGPUCommandQueueComplete();
	bool InitDirect3D();
	bool PostInitDirect3D();
private:
	ComPtr<IDXGIFactory4>	DXGIFactory;							// DX工厂
	ComPtr<ID3D12Device>	D3dDevice;								// DX驱动
	ComPtr<ID3D12Fence>		Fence;									// 资源围栏（用于CPU和GPU同步）
	ComPtr<ID3D12CommandQueue>			CommandQueue;				// 命令队列
	ComPtr<ID3D12CommandAllocator>		CommandAllocator;			// 命令分配器
	ComPtr<ID3D12GraphicsCommandList>	GraphicsCommandList;		// 命令列表
	ComPtr<IDXGISwapChain> SwapChain;								// 交换链
	ComPtr<ID3D12DescriptorHeap> RTVHeap;							// (render target view) 资源描述符堆
	ComPtr<ID3D12DescriptorHeap> DSVHeap;							// (depth stencil view) 资源描述符堆


	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;			// 交换链中存放的rtv类型的资源
	ComPtr<ID3D12Resource> DepthStencilBuffer;						// dsv类型的资源
	D3D12_VIEWPORT ViewPortInfo;									// Describes the dimensions of a viewport
	D3D12_RECT ViewPortRect;										// Describes the dimensions of a viewport
private:
	UINT CurrentFenceIndex;											// 当前的围栏值
	int CurrentSwapBufferIndex;										// 当前交换链中资源索引
	UINT M4XQualityLevels;											// 多重采样的质量等级
	bool bMSAA4XEnabled;											// 是否开启多重采样
	DXGI_FORMAT BackBufferFormat;									// 后台缓冲资源格式
	DXGI_FORMAT DepthStencilFormat;									// 深度模板资源格式
	UINT RTVDescriptorSize;											// rtv描述符大小
private:
	CMeshManage* MeshManage;										//mesh管理器
};