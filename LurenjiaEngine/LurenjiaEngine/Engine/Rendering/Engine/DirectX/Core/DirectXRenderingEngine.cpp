#include "DirectXRenderingEngine.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "../../../../Mesh/PlaneMesh.h"
#include "../../../../Mesh/SphereMesh.h"
#include "../../../../Mesh/Core/Material/Material.h"
#include "../../../../Actor/Light/ParallelLight.h"
#include "../../../../Actor/Light/SpotLight.h"
#include "../../../../Actor/Light/PointLight.h"
#include "../../../../Test/Texture/TextureTest.h"
#include "../../../../Test/Material/MaterialTest.h"
#include "../../../../Test/RenderLayer/TransparentTest.h"
#include "../../../../Mesh/BoxMesh.h"
#include "../../../../Actor/Sky/Fog.h"


enum class EMaterialType;
CDirectXRenderingEngine::CDirectXRenderingEngine()
	: ViewPortInfo({})
	, ViewPortRect({})
	, CurrentFenceIndex(0)
	, CurrentSwapBufferIndex(0)
	, M4XQualityLevels(0)
	, bMSAA4XEnabled(false)
	, BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
	, DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT)//24位深度缓冲区，映射到 0 - 1 之间。8位模板缓冲区，映射到0 - 255 之间。
	, RTVDescriptorSize(0)
{
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.emplace_back(ComPtr<ID3D12Resource>());
	}
	MeshManage =  LurenjiaEngine::CreateObject<CMeshManage>("MeshManage");
	LightManage = LurenjiaEngine::CreateObject<CLightManage>("LightManage");

	World = nullptr;

	FRenderLayerManage::GetRenderLayerManage();//创建层级必须在渲染模型之前
}

int CDirectXRenderingEngine::PreInit(FWinMainCommandParameters& InParameters)
{
	Engine_Log("DirectXRenderingEngine pre initialization complete.");
	return 1;
}

int CDirectXRenderingEngine::Init(FWinMainCommandParameters InParameters)
{

	if (InitDirect3D()){}
	if (PostInitDirect3D()){}
	MeshManage->Init();
	Engine_Log("Engine initialization complete");

	return 0;
}

int CDirectXRenderingEngine::PostInit()
{
	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), nullptr)); 
	//构建mesh
	//AMesh* BoxMesh = MeshManage->CreateBoxMesh("BoxMesh", 10, 10, 10);		//box 高度，宽度，深度
	//AMesh* SphereMesh = MeshManage->CreateSphereMesh("SphereMesh", 1, 20, 20);//半径，轴分割度，高度分割度
	//AMesh* CylinderMesh = MeshManage->CreateCylinderMesh("CylinderMesh", 2, 2, 20, 2, 2);//上半径，下半径，轴分割度，高度，高分割度
	//APlaneMesh* PlaneMesh = dynamic_cast<APlaneMesh*>(MeshManage->CreatePlaneMesh("PlaneMesh", 2, 2, 10, 10)) ;//高度，宽度，高度细分，宽度细分
	
	//BoxMesh->SetScale(XMFLOAT3(0.5, 1, 1));
	//SphereMesh->SetPosition(XMFLOAT3(0, 20, 0));
	//PlaneMesh->SetRotation(fvector_3d(0, -90, 0));
	
	//cubeMap
	if (ABoxMesh* CubeMesh = World->CreateActor<ABoxMesh>("ABoxMesh"))
	{
		CubeMesh->SetMeshComponent("ABoxMeshComponent", 10, 10, 10, EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND);
		CubeMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, 0.f));
		CubeMesh->SetComponentScale(XMFLOAT3(100.f, 100.f, 100.f));
		if (CMaterial* CubeMaterial = new CMaterial())
		{
			CubeMaterial->ResetGuid("CubeMaterial");//给创建的材质设置Guid
			CubeMaterial->SetMaterialTextureMapKey("x1_CubeMap");
			CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
			CubeMesh->SetSubMaterials(0, CubeMaterial);
		}
	}
	//平行光
	//if (AParallelLight* ParallelLight = World->CreateActor<AParallelLight>("AParallelLight"))
	//{
	//	ParallelLight->SetLightIntensity(XMFLOAT3(1.0f, 1.0f, 1.0f));
	//	ParallelLight->SetPosition(XMFLOAT3(-30.f, 0.f, 0.f));
	//	ParallelLight->SetRotation(fvector_3d(0.f, 0.f, 90.0f));
	//}
	//if (AParallelLight* ParallelLight2 = World->CreateActor<AParallelLight>("AParallelLight2"))
	//{
	//	ParallelLight2->SetLightIntensity(XMFLOAT3(1.f, 1.f, 1.f));
	//	ParallelLight2->SetPosition(XMFLOAT3(-20.f, 0.f, 10.f));
	//	ParallelLight2->SetRotation(fvector_3d(0.f, 90.f, 0.0f));
	//}
	//点光源
	if (APointLight* PointLight = World->CreateActor<APointLight>("APointLight"))
	{
		PointLight->SetLightIntensity(XMFLOAT3(1.f, 1.f, 1.f));
		PointLight->SetLightStartAttenuation(1.f);
		PointLight->SetLightEndAttenuation(140.f);
		PointLight->SetPosition(XMFLOAT3(-5.f, 0.f, 2.f));
		//SpotLight->SetRotation(fvector_3d(0.f, 0.f, 90.f));
	}
	//聚光灯
	//if (ASpotLight* SpotLight = World->CreateActor<ASpotLight>("ASpotLight"))
	//{
	//	SpotLight->SetLightIntensity(XMFLOAT3(1.f, 1.f, 1.f));
	//	SpotLight->SetPosition(XMFLOAT3(-15.f, 0.f, 15.f));
	//	SpotLight->SetRotation(fvector_3d(0.f, 90.f, 0.f));
	//	SpotLight->SetLightStartAttenuation(1.f);
	//	SpotLight->SetLightEndAttenuation(100.f);
	//	SpotLight->SetConicalInnerCorner(30.f);
	//	SpotLight->SetConicalOuterCorner(70.f);
	//}
	
	//雾
	if (AFog* Fog = World->CreateActor<AFog>("AFog"))
	{
		Fog->SetFogColor(fvector_color(0.7f));
		Fog->SetFogStart(10.f);
		Fog->SetFogRange(200.f);
		Fog->SetFogHeight(900.f);
		Fog->SetFogTransparentCoefficient(0.08f);
	}


	//MaterialTest::BuildMaterialTestData();
	TextureTest::BuildTextureTestData();
	TransparentTest::BuildTransparentTestData();
	
	

	MeshManage->BuildPipeline();

	GraphicsCommandList->Close();
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	WaitGPUCommandQueueComplete();

	Engine_Log("Engine post initialization complete");

	return 0;
}

void CDirectXRenderingEngine::Tick(float DeltaTime)
{
	//----------clear old different data start-----
	ANALYSIS_HRESULT(CommandAllocator->Reset());

	MeshManage->PreDraw(DeltaTime);

	D3D12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrier);

	GraphicsCommandList->RSSetViewports(1, &ViewPortInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewPortRect);
	//clear curr SwapChainResourceDescriptor
	GraphicsCommandList->ClearRenderTargetView(GetCurrentSwapBufferView(), DirectX::Colors::Black, 0, nullptr);
	//clear curr DepthSencilResourceDesciptor
	GraphicsCommandList->ClearDepthStencilView(GetCurrentDSBufferView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
	//----------clear old different data finish-----
	// 
		//给RenderTarget和DepthSencil设置Resource Descriptor handle
	D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE DSBufferView = GetCurrentDSBufferView();
	GraphicsCommandList->OMSetRenderTargets(1, &SwapBufferView, true, &DSBufferView);

	//Draw other content
	MeshManage->Draw(DeltaTime);	//将图形渲染命令添加到commandList中
	MeshManage->PostDraw(DeltaTime);

	D3D12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrier2);

	//close commandlist and commit to commandqueue
	GraphicsCommandList->Close();
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//将画面呈现到屏幕上
	SwapChain->Present(0, 0);
	//改变交换链索引
	CurrentSwapBufferIndex = (CurrentSwapBufferIndex + 1) % 2;

	//cpu等待gpu执行
	WaitGPUCommandQueueComplete();
}

int CDirectXRenderingEngine::PreExit()
{
	Engine_Log("Engine pre Exit complete");
	return 0;
}

int CDirectXRenderingEngine::Exit()
{
	PreExit();
	Engine_Log("Engine Exit complete");
	PostExit();
	return 0;
}

int CDirectXRenderingEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	Engine_Log("Engine post Exit complete");
	return 0;
}

void CDirectXRenderingEngine::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	MeshManage->UpdateConstantView(DeltaTime, ViewportInfo);
}

ComPtr<ID3D12Device> CDirectXRenderingEngine::GetD3dDevice() const
{
	return D3dDevice;
}

ComPtr<ID3D12GraphicsCommandList> CDirectXRenderingEngine::GetGraphicsCommandList() const
{
	return GraphicsCommandList;
}

ComPtr<ID3D12CommandAllocator> CDirectXRenderingEngine::GetCommandAllocator() const
{
	return CommandAllocator;
}

DXGI_FORMAT CDirectXRenderingEngine::GetBackBufferFormat() const
{
	return BackBufferFormat;
}

DXGI_FORMAT CDirectXRenderingEngine::GetDepthStencilFormat() const
{
	return DepthStencilFormat;
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentDSBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(DSVHeap->GetCPUDescriptorHandleForHeapStart());
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(RTVHeap->GetCPUDescriptorHandleForHeapStart(), CurrentSwapBufferIndex, RTVDescriptorSize);
}

ID3D12Resource* CDirectXRenderingEngine::GetCurrentSwapBuffer() const
{
	return SwapChainBuffer[CurrentSwapBufferIndex].Get();
}

UINT CDirectXRenderingEngine::GetDXGISampleCount() const
{
	return bMSAA4XEnabled ? 4 : 1;
}

UINT CDirectXRenderingEngine::GetDXGISampleQuality() const
{
	return bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
}

void CDirectXRenderingEngine::SetWorld(CWorld* InWorld)
{
	World = InWorld;
}

CWorld* CDirectXRenderingEngine::GetWorld() const
{
	return World;
}

void CDirectXRenderingEngine::WaitGPUCommandQueueComplete()
{
	//增加围栏值，，，将接下来的命令标记到此围栏点
	CurrentFenceIndex++;
	//向命令队列中添加设置围栏点的命令，并且由gpu执行
	//在gpu处理完此signal命令之前的命令，，之前不会设置新的围栏点
	ANALYSIS_HRESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));
	//cpu等待gpu，直到后者完成此围栏点之前的所有命令
	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		HANDLE eventHandle = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
		//如果gpu执行了signal命令，则激发预定事件
		ANALYSIS_HRESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, eventHandle));
		//等待gpu执行signal命令，触发事件
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

bool CDirectXRenderingEngine::InitDirect3D()
{
#if defined(_DEBUG)
	//添加调试信息
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(D3D12Debug.GetAddressOf()))))
	{
		D3D12Debug->EnableDebugLayer();
	}
#endif // define(_DEBUG)

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

	HRESULT D3dDeviceResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
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
		//命令队列描述
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	QueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	QueueDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CommandAllocator.GetAddressOf())));
	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0,//默认单个gpu
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),//关联到命令分配器
		nullptr,//ID3D12PipelineState 管线状态
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())
	));
	//GraphicsCommandList->Close();
//----------创建命令完成GraphicsCommandList, CommandQueue, CommandAllocator-----
//----------检测当前驱动对多重采样的支持-----
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;
	QualityLevels.SampleCount = 4;
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;
	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&QualityLevels,
		sizeof(QualityLevels)));
	M4XQualityLevels = QualityLevels.NumQualityLevels;
	//----------检测当前驱动对多重采样的支持结束-----
	//----------创建交换链-----
	SwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;//分子
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;//分母
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	/*
	* //交换链的用途
	* DXGI_USAGE_BACK_BUFFER		被用作后台缓冲区
	* DXGI_USAGE_READ_ONLY			交换链只读
	* DXGI_USAGE_SHADER_INPUT		被用作着色器输入
	* DXGI_USAGE_SHARED				共享
	* DXGI_USAGE_UNORDERED_ACCESS	无序访问
	*/
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//被用作渲染到目标
	SwapChainDesc.OutputWindow = MainWindowsHandle;				//指定windows句柄
	SwapChainDesc.Windowed = true;								//以窗口运行
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;//将旧的交换链丢弃
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//自由切换窗口
	//格式纹理
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;
	//多重采样在交换链中的设置
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();
	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));
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
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));

	//create Depth stencil View
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.NumDescriptors = 1;
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));
	//----------创建完成资源描述堆-----
	return true;
}

bool CDirectXRenderingEngine::PostInitDirect3D()
{
	//cpu等待gpu执行
	WaitGPUCommandQueueComplete();

//----------为交换链中两个缓冲区创建资源描述符开始-----
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
		BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//从资源描述堆中拿到第一个资源描述符，，，通过句柄的形式引用资源描述符
	CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		//获得交换链中第i个缓冲区
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		//为此缓冲区创建一个RTV（资源描述符）
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, RTVHandle);
		RTVHandle.Offset(1, RTVDescriptorSize);
	}
//----------为交换链中两个缓冲区创建资源描述符开始-----
//----------创建深度模板缓冲区及其资源描述符开始-----
	DepthStencilBuffer.Reset();
	D3D12_RESOURCE_DESC ResourceDesc;//depthstencilbuffer的描述
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ResourceDesc.Alignment = 0;//资源的对齐方式
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = GetDXGISampleCount();
	ResourceDesc.SampleDesc.Quality = GetDXGISampleQuality();
	ResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;//depthstencilbuffer的清除规则描述
	ClearValue.DepthStencil.Depth = 1;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DepthStencilFormat;

	//创建堆的属性
	/*
	* D3D12_HEAP_TYPE_DEFAULT 默认堆，只可以gpu访问，比如DepthStencilBuffer
	* D3D12_HEAP_TYPE_UPLOAD 上传堆，向此堆里的提交都必须是由cpu提交上来的
	* D3D12_HEAP_TYPE_READBACK	回读堆，向此堆里的提交都是由cpu读取的资源
	*/
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//gpu资源都存储在堆中
	//创建一个深度模板缓冲区和一个堆，，，将此缓冲区提交到堆中
	D3dDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	DSVDesc.Format = DepthStencilFormat;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;//深度模板缓冲的尺寸

	D3D12_CPU_DESCRIPTOR_HANDLE DSVHandle = DSVHeap->GetCPUDescriptorHandleForHeapStart();
	//创建深度模板资源描述符
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHandle);
//----------创建深度模板缓冲区及其资源描述符结束-----

	//向命令列表中添加命令，在添加完成必须记得关闭命令列表
	//设置资源屏障，也就是将切换资源的状态，
	//现在将深度模板缓冲从状态1（访问不同引擎资源）transition to 状态2（深度模板可写状态）
	CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrier);
	GraphicsCommandList->Close();

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	//将命令列表里的命令一条一条的添加到命令队列中，，，也就是提交命令列表
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//这些会覆盖原先的windows画布
	ViewPortInfo.TopLeftX = 0;
	ViewPortInfo.TopLeftY = 0;
	ViewPortInfo.Width = (float)FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewPortInfo.Height = (float)FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewPortInfo.MinDepth = 0;
	ViewPortInfo.MaxDepth = 1;

	//设置视口的大小，原点在左上角
	ViewPortRect.left = 0;
	ViewPortRect.top = 0;
	ViewPortRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewPortRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

	//cpu等待gpu执行
	WaitGPUCommandQueueComplete();
	return true;
}
