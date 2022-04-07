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
	//��ʼ����־ϵͳ
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log PreInit.");
	Engine_Log("Engine pre initialization complete");
	return 0;
}
int FWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	PreInit(InParameters);
	//��������
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
//----------��ʼ��������buffer��ʼ-----
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
		//������������Դ��������������̨��ǰ̨����
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, RTVHandle);
		RTVHandle.Offset(1, RTVDescriptorSize);
	}
//----------��ʼ��������buffer����-----
//----------��ʼ�����ģ��buffer��ʼ-----
	D3D12_RESOURCE_DESC ResourceDesc;//depthstencilbuffer������
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ResourceDesc.Alignment = 0;//��Դ�Ķ��뷽ʽ
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;//depthstencilbuffer�������������
	ClearValue.DepthStencil.Depth = 1;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DepthStencilFormat;
	
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(//������d3dDevice�ύDepthStencilBuffer��Դ
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
	//�������ģ����Դ������
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHandle);
//----------��ʼ�����ģ��buffer����-----

	CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GraphicsCommamdList->ResourceBarrier(1,&ResourceBarrier);
	GraphicsCommamdList->Close();

	//�ύ����
	ID3D12CommandList* CommandList[] = { GraphicsCommamdList.Get() };
	//����ָ��������Ȼ��ִ��
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
	//��ӵ�����Ϣ
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(D3D12Debug.GetAddressOf()))))
	{
		D3D12Debug->EnableDebugLayer();
	}
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
	HRESULT CMDResult = D3dDevice->CreateCommandList(
		0,//Ĭ�ϵ���gpu
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),//���������������
		NULL,//ID3D12PipelineState ����״̬
		IID_PPV_ARGS(GraphicsCommamdList.GetAddressOf())
	);
	if (FAILED(CMDResult))
	{
		Engine_Log_Error("create GraphicsCommandList failed = %i", (int)CMDResult);
	}
	GraphicsCommamdList->Close();
//----------�����������GraphicsCommandList, CommandQueue, CommandAllocator-----
//----------�������ز���-----
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
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;//���ɵĽ���������
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//�����л�����
	//��ʽ����
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;
	//���ز����ڽ������е�����
	SwapChainDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	SwapChainDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	HRESULT ResultSwapChain = DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf());
	if (FAILED(ResultSwapChain))
	{
		Engine_Log_Error("create SwapChain failed = %i", (int)ResultSwapChain);
	}
//----------������ɽ�����-----
//----------������Դ������-----
	/*
	* D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��constant buffer view,shader resource view, unorder access view��
	* D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ������������
	* D3D12_DESCRIPTOR_HEAP_TYPE_RTV ��Ⱦ��Ŀ��������
	* D3D12_DESCRIPTOR_HEAP_TYPE_DSV ���ģ��������
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
//----------���������Դ������-----
	return true;
}
#endif
