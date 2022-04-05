#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"
FWindowsEngine::FWindowsEngine()
{
	M4XQualityLevels = 0;
	bMSAA4XEnabled = false;
	BufferFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
}
int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{

	//初始化日志系统
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	//处理命令

	if (InitWindows(InParameters))
	{

	}
	if (InitDirect3D())
	{

	}

	Engine_Log("Engine pre initialization complete");
	return 0;
}
int FWindowsEngine::Init()
{
	Engine_Log("Engine initialization complete");
	return 0;
}
int FWindowsEngine::PostInit()
{
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
	Engine_Log("Engine Exit complete");
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
	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0,//默认单个gpu
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),//关联到命令分配器
		NULL,//ID3D12PipelineState 管线状态
		IID_PPV_ARGS(GraphicsCommamdList.GetAddressOf())
	));
	GraphicsCommamdList->Close();
//----------创建命令完成GraphicsCommandList, CommandQueue, CommandAllocator-----
//----------创建多重采样-----
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.SampleCount = 4;
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;
	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE::D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&QualityLevels,
		sizeof(QualityLevels)
	));
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
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;//将旧的交换链丢弃
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//自由切换窗口
	//格式纹理
	SwapChainDesc.BufferDesc.Format = BufferFormat;
	//多重采样在交换链中的设置
	SwapChainDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	SwapChainDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));
//----------创建完成交换链-----
	
	return true;
}
#endif
