#pragma
#include "../../Core/RenderingEngine.h"
#include "../../../../Core/World.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../Manage/LightManage.h"
#include "../../../../Rendering/Core/DirectX/RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"

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
	void StartSetMainViewportRenderTarget();// 开始设置主视口的rendertarget
	void EndSetMainViewportRenderTarget();// 结束设置主视口的rendertarget
	void ClearMainSwapChain();// 清除主视口的交换链

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
	//设置世界
	void SetWorld(CWorld* InWorld);
	//获取世界
	CWorld* GetWorld() const;
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
	ComPtr<ID3D12DescriptorHeap> RTVHeap;							// RTV描述符堆
	ComPtr<ID3D12DescriptorHeap> DSVHeap;							// DSV描述符堆
	FDirectXDescriptorHeap DescriptorHeap;							// CBV_SRV_UAV这种描述符堆，程序所有的cbv srv uav资源都应该放在这个堆里面


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
	CLightManage* LightManage;										//灯光管理器
	CWorld* World;													//世界
};