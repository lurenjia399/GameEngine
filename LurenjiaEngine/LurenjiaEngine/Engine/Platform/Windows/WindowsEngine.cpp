#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"
FWindowsEngine::FWindowsEngine()
{
	M4XQualityLevels = 0;
	bMSAA4XEnabled = false;
	BackBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	DepthStencilFormat = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	RTVDescriptorSize = 0;
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.emplace_back(ComPtr<ID3D12Resource>());
	}
	
}
int FWindowsEngine::PreInit(FWinMainCommandParameters& InParameters)
{
	//初始化日志系统
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log PreInit.");
	Engine_Log("Engine pre initialization complete");
	return 0;
}
int FWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	PreInit(InParameters);
	//处理命令
	if (InitWindows(InParameters))
	{

	}
	if (InitDirect3D())
	{

	}
	Engine_Log("Engine initialization complete");
	PostInit();
	return 0;
}
int FWindowsEngine::PostInit()
{
//----------初始化交换链buffer开始-----
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();
	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
		BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		//创建交换链资源描述符，，，后台和前台两个
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, RTVHandle);
		RTVHandle.Offset(1, RTVDescriptorSize);
	}
//----------初始化交换链buffer结束-----
//----------初始化深度模板buffer开始-----
	D3D12_RESOURCE_DESC ResourceDesc;//depthstencilbuffer的描述
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ResourceDesc.Alignment = 0;//资源的对齐方式
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;//depthstencilbuffer的清除规则描述
	ClearValue.DepthStencil.Depth = 1;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DepthStencilFormat;
	
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(//首先用d3dDevice提交DepthStencilBuffer资源
		&HeapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON,
		&ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	DSVDesc.Format = DepthStencilFormat;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

	D3D12_CPU_DESCRIPTOR_HANDLE DSVHandle = DSVHeap->GetCPUDescriptorHandleForHeapStart();
	//创建深度模板资源描述符
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHandle);
//----------初始化深度模板buffer结束-----

	CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GraphicsCommamdList->ResourceBarrier(1,&ResourceBarrier);
	GraphicsCommamdList->Close();

	//提交命令
	ID3D12CommandList* CommandList[] = { GraphicsCommamdList.Get() };
	//计算指令数量，然后执行
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	Engine_Log("Engine post initialization complete");
	return 0;
}
void FWindowsEngine::Tick()
{
}
int FWindowsEngine::PreExit()
{
	Engine_Log("Engine pre Exit complete");
	return 0;
}
int FWindowsEngine::Exit()
{
	PreExit();
	Engine_Log("Engine Exit complete");
	PostExit();
	return 0;
}
int FWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	Engine_Log("Engine post Exit complete");
	return 0;
}
bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	// 注册窗口
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);			//该对象实际占用多大内存
	WindowsClass.cbClsExtra = 0;						//是否需要额外空间
	WindowsClass.cbWndExtra = 0;						//是否需要额外内存
	WindowsClass.hbrBackground = nullptr;				//如果有设置就是GDI擦除
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//设置一个箭头光标
	WindowsClass.hIcon = nullptr;						//应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = nullptr;						//应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance;	//传递自己的窗口实例
	WindowsClass.lpszClassName = L"LurenjiaEngine";		//窗口名称
	WindowsClass.lpszMenuName = nullptr;				//设置菜单的名称
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;		//怎么绘制窗口，垂直或者水平
	WindowsClass.lpfnWndProc = EngineWindowProc;		//绑定消息处理函数

	//注册窗口
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		// 如果注册失败
		Engine_Log_Error("Register windows class failed.");
		MessageBox(NULL, L"Register windows class fail,", L"Error", MB_OK);
	}

	/*
	* {left, top, right, bottom}
	*/
	RECT Rect = { 0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	/*
	* 创建的视口
	* WS_OVERLAPPEDWINDOW 视口风格
	* 是否有菜单
	*/
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int WindowWidth = Rect.right - Rect.left;
	int WindowHeight = Rect.bottom - Rect.top;

	MyWindowsHandle = CreateWindowEx(
		NULL,						//窗口额外风格
		L"LurenjiaEngine",			//窗口名称
		L"Lurenjia Engine",			//窗口名称(显示在窗口的标题栏上)
		WS_OVERLAPPEDWINDOW,		//窗口风格
		100, 100,					//窗口左上角起始位置
		WindowWidth, WindowHeight,	//窗口长宽
		NULL,						//副窗口句柄
		nullptr,					//窗口菜单句柄
		InParameters.HInstance,		//窗口实例
		NULL						//可以往窗口中传参数
		);
	if (!MyWindowsHandle)
	{
		Engine_Log_Error("CreateWindow failed.");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}
	
	//显示窗口
	ShowWindow(MyWindowsHandle, SW_SHOW);
	//窗口是脏的就刷一下
	UpdateWindow(MyWindowsHandle);
	Engine_Log("InitWindows complete.");
	return true;
}
bool FWindowsEngine::InitDirect3D()
{
	//添加调试信息
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(D3D12Debug.GetAddressOf()))))
	{
		D3D12Debug->EnableDebugLayer();
	}
//----------创建DXGIFactory, D3dDevice, Fence-----
	//HRESULT			返回值	是一个ENUM
	//S_OK				0x00000000 成功
	//E_UNEXPECTED		0x8000FFFF 意外失败
	//E_NOTIMPL			0x80004001 未实现
	//E_OUTOFMEMORY		0x8007000E 未能分配所需内存
	//E_INVALIDARG		0x80070057 一个或多个参数无效
	//E_NOINTERFACE		0x80004002 不支持此接口
	//E_POINTER			0x80004003 无效指针
	//E_HANDLE			0x80070006 无效句柄
	//E_ABORT			0x80004004 操作终止
	//E_FAIL			0x80004005 错误
	//E_ACCESSDENIED	0x80070005 一般的访问被拒绝
	ANALYSIS_HRESULT(CreateDXGIFactory(IID_PPV_ARGS(&DXGIFactory)));

	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// 如果没法创建基于硬件的Device，那就创建一个
		// WARP(Windows Advanced Rasterization Platform)软件适配器的Device
		// DXGI是微软提供的一个基础库，它提供了与图形API有关但是又不适合放入图形API的功能
		// 比如枚举系统中的显示适配器，显示器，支持的显示分辨率刷新率等等。
		// 这里我们用到的一个重要的接口是IDXGIFactory4，因为年代久远的关系，微软都不得不用这种
		// 魔数了。这个接口可以枚举显示适配器，同样，创建交换链也是它的功能。
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	//第二个参数: enum
	//D3D12_FENCE_FLAG_NONE						不共享Fence
	//D3D12_FENCE_FLAG_SHARED					共享Fence
	//D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER		跟适配器共享
	//D3D12_FENCE_FLAG_NON_MONITORED
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
//----------创建完成DXGIFactory, D3dDevice, Fence-----
//----------创建命令GraphicsCommandList, CommandQueue, CommandAllocator-----
	//初始化命令对象
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	QueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	QueueDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CommandAllocator.GetAddressOf())));
	HRESULT CMDResult = D3dDevice->CreateCommandList(
		0,//默认单个gpu
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),//关联到命令分配器
		NULL,//ID3D12PipelineState 管线状态
		IID_PPV_ARGS(GraphicsCommamdList.GetAddressOf())
	);
	if (FAILED(CMDResult))
	{
		Engine_Log_Error("create GraphicsCommandList failed = %i", (int)CMDResult);
	}
	GraphicsCommamdList->Close();
//----------创建命令完成GraphicsCommandList, CommandQueue, CommandAllocator-----
//----------创建多重采样-----
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;
	QualityLevels.SampleCount = 4;
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;
	HRESULT ResultFeatureSupport = D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE::D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&QualityLevels,
		sizeof(QualityLevels));
	if (FAILED(ResultFeatureSupport))
	{
		Engine_Log_Error("create FeaTureSupport failed = %i", (int)ResultFeatureSupport);
	}
	M4XQualityLevels = QualityLevels.NumQualityLevels;
//----------创建完成多重采样-----
//----------创建交换链-----
	SwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	/*
	* DXGI_USAGE_BACK_BUFFER		使用表面或资源作为后台缓冲区渲染目标
	* DXGI_USAGE_READ_ONLY			只读
	* DXGI_USAGE_SHADER_INPUT		使用表面或资源作为着色器的输入
	* DXGI_USAGE_SHARED				共享
	* DXGI_USAGE_UNORDERED_ACCESS	使用表面或资源进行无序访问
	*/
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//使用表面或资源作为输出渲染目标
	SwapChainDesc.OutputWindow = MyWindowsHandle;				//指定windows句柄
	SwapChainDesc.Windowed = true;								//以窗口运行
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;//将旧的交换链丢弃
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//自由切换窗口
	//格式纹理
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;
	//多重采样在交换链中的设置
	SwapChainDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	SwapChainDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	HRESULT ResultSwapChain = DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf());
	if (FAILED(ResultSwapChain))
	{
		Engine_Log_Error("create SwapChain failed = %i", (int)ResultSwapChain);
	}
//----------创建完成交换链-----
//----------创建资源描述堆-----
	/*
	* D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV（constant buffer view,shader resource view, unorder access view）
	* D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER 采样器描述符
	* D3D12_DESCRIPTOR_HEAP_TYPE_RTV 渲染到目标描述符
	* D3D12_DESCRIPTOR_HEAP_TYPE_DSV 深度模板描述符
	*/
	//create Runtime Target view
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));

	//create Depth stencil View
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.NumDescriptors = 1;
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));
//----------创建完成资源描述堆-----
	return true;
}
#endif
