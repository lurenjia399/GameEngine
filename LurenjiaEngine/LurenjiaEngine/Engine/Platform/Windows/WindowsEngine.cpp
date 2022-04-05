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

	//��ʼ����־ϵͳ
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	//��������

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
	// ע�ᴰ��
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);			//�ö���ʵ��ռ�ö���ڴ�
	WindowsClass.cbClsExtra = 0;						//�Ƿ���Ҫ����ռ�
	WindowsClass.cbWndExtra = 0;						//�Ƿ���Ҫ�����ڴ�
	WindowsClass.hbrBackground = nullptr;				//��������þ���GDI����
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//����һ����ͷ���
	WindowsClass.hIcon = nullptr;						//Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = nullptr;						//Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance;	//�����Լ��Ĵ���ʵ��
	WindowsClass.lpszClassName = L"LurenjiaEngine";		//��������
	WindowsClass.lpszMenuName = nullptr;				//���ò˵�������
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;		//��ô���ƴ��ڣ���ֱ����ˮƽ
	WindowsClass.lpfnWndProc = EngineWindowProc;		//����Ϣ������

	//ע�ᴰ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		// ���ע��ʧ��
		Engine_Log_Error("Register windows class failed.");
		MessageBox(NULL, L"Register windows class fail,", L"Error", MB_OK);
	}

	/*
	* {left, top, right, bottom}
	*/
	RECT Rect = { 0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	/*
	* �������ӿ�
	* WS_OVERLAPPEDWINDOW �ӿڷ��
	* �Ƿ��в˵�
	*/
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int WindowWidth = Rect.right - Rect.left;
	int WindowHeight = Rect.bottom - Rect.top;

	MyWindowsHandle = CreateWindowEx(
		NULL,						//���ڶ�����
		L"LurenjiaEngine",			//��������
		L"Lurenjia Engine",			//��������(��ʾ�ڴ��ڵı�������)
		WS_OVERLAPPEDWINDOW,		//���ڷ��
		100, 100,					//�������Ͻ���ʼλ��
		WindowWidth, WindowHeight,	//���ڳ���
		NULL,						//�����ھ��
		nullptr,					//���ڲ˵����
		InParameters.HInstance,		//����ʵ��
		NULL						//�����������д�����
		);
	if (!MyWindowsHandle)
	{
		Engine_Log_Error("CreateWindow failed.");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}
	
	//��ʾ����
	ShowWindow(MyWindowsHandle, SW_SHOW);
	//��������ľ�ˢһ��
	UpdateWindow(MyWindowsHandle);
	Engine_Log("InitWindows complete.");
	return true;
}
bool FWindowsEngine::InitDirect3D()
{
//----------����DXGIFactory, D3dDevice, Fence-----
	//HRESULT			����ֵ	��һ��ENUM
	//S_OK				0x00000000 �ɹ�
	//E_UNEXPECTED		0x8000FFFF ����ʧ��
	//E_NOTIMPL			0x80004001 δʵ��
	//E_OUTOFMEMORY		0x8007000E δ�ܷ��������ڴ�
	//E_INVALIDARG		0x80070057 һ������������Ч
	//E_NOINTERFACE		0x80004002 ��֧�ִ˽ӿ�
	//E_POINTER			0x80004003 ��Чָ��
	//E_HANDLE			0x80070006 ��Ч���
	//E_ABORT			0x80004004 ������ֹ
	//E_FAIL			0x80004005 ����
	//E_ACCESSDENIED	0x80070005 һ��ķ��ʱ��ܾ�
	ANALYSIS_HRESULT(CreateDXGIFactory(IID_PPV_ARGS(&DXGIFactory)));

	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// ���û����������Ӳ����Device���Ǿʹ���һ��
		// WARP(Windows Advanced Rasterization Platform)�����������Device
		// DXGI��΢���ṩ��һ�������⣬���ṩ����ͼ��API�йص����ֲ��ʺϷ���ͼ��API�Ĺ���
		// ����ö��ϵͳ�е���ʾ����������ʾ����֧�ֵ���ʾ�ֱ���ˢ���ʵȵȡ�
		// ���������õ���һ����Ҫ�Ľӿ���IDXGIFactory4����Ϊ�����Զ�Ĺ�ϵ��΢�����ò�������
		// ħ���ˡ�����ӿڿ���ö����ʾ��������ͬ��������������Ҳ�����Ĺ��ܡ�
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	//�ڶ�������: enum
	//D3D12_FENCE_FLAG_NONE						������Fence
	//D3D12_FENCE_FLAG_SHARED					����Fence
	//D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER		������������
	//D3D12_FENCE_FLAG_NON_MONITORED
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
//----------�������DXGIFactory, D3dDevice, Fence-----
//----------��������GraphicsCommandList, CommandQueue, CommandAllocator-----
	//��ʼ���������
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	QueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	QueueDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CommandAllocator.GetAddressOf())));
	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0,//Ĭ�ϵ���gpu
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),//���������������
		NULL,//ID3D12PipelineState ����״̬
		IID_PPV_ARGS(GraphicsCommamdList.GetAddressOf())
	));
	GraphicsCommamdList->Close();
//----------�����������GraphicsCommandList, CommandQueue, CommandAllocator-----
//----------�������ز���-----
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
//----------������ɶ��ز���-----
//----------����������-----
	SwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	/*
	* DXGI_USAGE_BACK_BUFFER		ʹ�ñ������Դ��Ϊ��̨��������ȾĿ��
	* DXGI_USAGE_READ_ONLY			ֻ��
	* DXGI_USAGE_SHADER_INPUT		ʹ�ñ������Դ��Ϊ��ɫ��������
	* DXGI_USAGE_SHARED				����
	* DXGI_USAGE_UNORDERED_ACCESS	ʹ�ñ������Դ�����������
	*/
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//ʹ�ñ������Դ��Ϊ�����ȾĿ��
	SwapChainDesc.OutputWindow = MyWindowsHandle;				//ָ��windows���
	SwapChainDesc.Windowed = true;								//�Դ�������
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;//���ɵĽ���������
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//�����л�����
	//��ʽ����
	SwapChainDesc.BufferDesc.Format = BufferFormat;
	//���ز����ڽ������е�����
	SwapChainDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	SwapChainDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));
//----------������ɽ�����-----
	
	return true;
}
#endif
