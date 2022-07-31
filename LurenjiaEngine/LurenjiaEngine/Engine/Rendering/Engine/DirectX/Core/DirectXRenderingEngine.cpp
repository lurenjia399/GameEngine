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
	, DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT)//24λ��Ȼ�������ӳ�䵽 0 - 1 ֮�䡣8λģ�建������ӳ�䵽0 - 255 ֮�䡣
	, RTVDescriptorSize(0)
{
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.emplace_back(ComPtr<ID3D12Resource>());
	}
	MeshManage =  LurenjiaEngine::CreateObject<CMeshManage>("MeshManage");
	LightManage = LurenjiaEngine::CreateObject<CLightManage>("LightManage");

	World = nullptr;

	FRenderLayerManage::GetRenderLayerManage();//�����㼶��������Ⱦģ��֮ǰ
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
	//����mesh
	//AMesh* BoxMesh = MeshManage->CreateBoxMesh("BoxMesh", 10, 10, 10);		//box �߶ȣ���ȣ����
	//AMesh* SphereMesh = MeshManage->CreateSphereMesh("SphereMesh", 1, 20, 20);//�뾶����ָ�ȣ��߶ȷָ��
	//AMesh* CylinderMesh = MeshManage->CreateCylinderMesh("CylinderMesh", 2, 2, 20, 2, 2);//�ϰ뾶���°뾶����ָ�ȣ��߶ȣ��߷ָ��
	//APlaneMesh* PlaneMesh = dynamic_cast<APlaneMesh*>(MeshManage->CreatePlaneMesh("PlaneMesh", 2, 2, 10, 10)) ;//�߶ȣ���ȣ��߶�ϸ�֣����ϸ��
	
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
			CubeMaterial->ResetGuid("CubeMaterial");//�������Ĳ�������Guid
			CubeMaterial->SetMaterialTextureMapKey("x1_CubeMap");
			CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
			CubeMesh->SetSubMaterials(0, CubeMaterial);
		}
	}
	//ƽ�й�
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
	//���Դ
	if (APointLight* PointLight = World->CreateActor<APointLight>("APointLight"))
	{
		PointLight->SetLightIntensity(XMFLOAT3(1.f, 1.f, 1.f));
		PointLight->SetLightStartAttenuation(1.f);
		PointLight->SetLightEndAttenuation(140.f);
		PointLight->SetPosition(XMFLOAT3(-5.f, 0.f, 2.f));
		//SpotLight->SetRotation(fvector_3d(0.f, 0.f, 90.f));
	}
	//�۹��
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
	
	//��
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
		//��RenderTarget��DepthSencil����Resource Descriptor handle
	D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE DSBufferView = GetCurrentDSBufferView();
	GraphicsCommandList->OMSetRenderTargets(1, &SwapBufferView, true, &DSBufferView);

	//Draw other content
	MeshManage->Draw(DeltaTime);	//��ͼ����Ⱦ������ӵ�commandList��
	MeshManage->PostDraw(DeltaTime);

	D3D12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrier2);

	//close commandlist and commit to commandqueue
	GraphicsCommandList->Close();
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//��������ֵ���Ļ��
	SwapChain->Present(0, 0);
	//�ı佻��������
	CurrentSwapBufferIndex = (CurrentSwapBufferIndex + 1) % 2;

	//cpu�ȴ�gpuִ��
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
	//����Χ��ֵ���������������������ǵ���Χ����
	CurrentFenceIndex++;
	//������������������Χ��������������gpuִ��
	//��gpu�������signal����֮ǰ�������֮ǰ���������µ�Χ����
	ANALYSIS_HRESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));
	//cpu�ȴ�gpu��ֱ��������ɴ�Χ����֮ǰ����������
	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		HANDLE eventHandle = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
		//���gpuִ����signal����򼤷�Ԥ���¼�
		ANALYSIS_HRESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, eventHandle));
		//�ȴ�gpuִ��signal��������¼�
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

bool CDirectXRenderingEngine::InitDirect3D()
{
#if defined(_DEBUG)
	//��ӵ�����Ϣ
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(D3D12Debug.GetAddressOf()))))
	{
		D3D12Debug->EnableDebugLayer();
	}
#endif // define(_DEBUG)

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

	HRESULT D3dDeviceResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
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
		//�����������
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	QueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	QueueDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CommandAllocator.GetAddressOf())));
	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0,//Ĭ�ϵ���gpu
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),//���������������
		nullptr,//ID3D12PipelineState ����״̬
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())
	));
	//GraphicsCommandList->Close();
//----------�����������GraphicsCommandList, CommandQueue, CommandAllocator-----
//----------��⵱ǰ�����Զ��ز�����֧��-----
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
	//----------��⵱ǰ�����Զ��ز�����֧�ֽ���-----
	//----------����������-----
	SwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;//����
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;//��ĸ
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	/*
	* //����������;
	* DXGI_USAGE_BACK_BUFFER		��������̨������
	* DXGI_USAGE_READ_ONLY			������ֻ��
	* DXGI_USAGE_SHADER_INPUT		��������ɫ������
	* DXGI_USAGE_SHARED				����
	* DXGI_USAGE_UNORDERED_ACCESS	�������
	*/
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//��������Ⱦ��Ŀ��
	SwapChainDesc.OutputWindow = MainWindowsHandle;				//ָ��windows���
	SwapChainDesc.Windowed = true;								//�Դ�������
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;//���ɵĽ���������
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//�����л�����
	//��ʽ����
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;
	//���ز����ڽ������е�����
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();
	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));
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
	//----------���������Դ������-----
	return true;
}

bool CDirectXRenderingEngine::PostInitDirect3D()
{
	//cpu�ȴ�gpuִ��
	WaitGPUCommandQueueComplete();

//----------Ϊ������������������������Դ��������ʼ-----
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
	//����Դ���������õ���һ����Դ������������ͨ���������ʽ������Դ������
	CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		//��ý������е�i��������
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		//Ϊ�˻���������һ��RTV����Դ��������
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, RTVHandle);
		RTVHandle.Offset(1, RTVDescriptorSize);
	}
//----------Ϊ������������������������Դ��������ʼ-----
//----------�������ģ�建����������Դ��������ʼ-----
	DepthStencilBuffer.Reset();
	D3D12_RESOURCE_DESC ResourceDesc;//depthstencilbuffer������
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ResourceDesc.Alignment = 0;//��Դ�Ķ��뷽ʽ
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = GetDXGISampleCount();
	ResourceDesc.SampleDesc.Quality = GetDXGISampleQuality();
	ResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;//depthstencilbuffer�������������
	ClearValue.DepthStencil.Depth = 1;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DepthStencilFormat;

	//�����ѵ�����
	/*
	* D3D12_HEAP_TYPE_DEFAULT Ĭ�϶ѣ�ֻ����gpu���ʣ�����DepthStencilBuffer
	* D3D12_HEAP_TYPE_UPLOAD �ϴ��ѣ���˶�����ύ����������cpu�ύ������
	* D3D12_HEAP_TYPE_READBACK	�ض��ѣ���˶�����ύ������cpu��ȡ����Դ
	*/
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//gpu��Դ���洢�ڶ���
	//����һ�����ģ�建������һ���ѣ��������˻������ύ������
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
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;//���ģ�建��ĳߴ�

	D3D12_CPU_DESCRIPTOR_HANDLE DSVHandle = DSVHeap->GetCPUDescriptorHandleForHeapStart();
	//�������ģ����Դ������
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHandle);
//----------�������ģ�建����������Դ����������-----

	//�������б����������������ɱ���ǵùر������б�
	//������Դ���ϣ�Ҳ���ǽ��л���Դ��״̬��
	//���ڽ����ģ�建���״̬1�����ʲ�ͬ������Դ��transition to ״̬2�����ģ���д״̬��
	CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrier);
	GraphicsCommandList->Close();

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	//�������б��������һ��һ������ӵ���������У�����Ҳ�����ύ�����б�
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//��Щ�Ḳ��ԭ�ȵ�windows����
	ViewPortInfo.TopLeftX = 0;
	ViewPortInfo.TopLeftY = 0;
	ViewPortInfo.Width = (float)FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewPortInfo.Height = (float)FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewPortInfo.MinDepth = 0;
	ViewPortInfo.MaxDepth = 1;

	//�����ӿڵĴ�С��ԭ�������Ͻ�
	ViewPortRect.left = 0;
	ViewPortRect.top = 0;
	ViewPortRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewPortRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

	//cpu�ȴ�gpuִ��
	WaitGPUCommandQueueComplete();
	return true;
}
