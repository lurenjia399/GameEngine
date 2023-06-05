#include "DynamicCubeMap.h"
#include "../../../../../Core/Viewport/ClientViewport.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Core/Construction/ObjectConstruction.h"
#include "../RenderTarget/CubeMapRenderTarget.h"



FDynamicCubeMap::FDynamicCubeMap()
	: Viewport({})
	, GeometryMap(nullptr)
	, DirectXPiepelineState(nullptr)
	, Width(512)
	, Height(512)
{
	RenderTarget = make_shared<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	if (Viewport.size() == 6)
	{
		for (size_t i = 0; i < 6; i++)
		{
			FViewportInfo MyViewportInfo = {};
			MyViewportInfo.cameraPosition = XMFLOAT4(Viewport[i]->GetPosition().x, Viewport[i]->GetPosition().y, Viewport[i]->GetPosition().z, 1.0f);
			MyViewportInfo.ViewMatrix = Viewport[i]->ViewMatrix;
			MyViewportInfo.ProjectMatrix = Viewport[i]->ProjectMatrix;
			GeometryMap->UpdateViewportConstantBufferView(DeltaTime, MyViewportInfo, i + 1);
		}
	}
}

void FDynamicCubeMap::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	GeometryMap = InGeometryMap;
	DirectXPiepelineState = InDirectXPiepelineState;
}

void FDynamicCubeMap::Draw(float DeltaTime)
{
	D3D12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		RenderTarget->GetRenderTarget(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);

	D3D12_VIEWPORT ViewPort_temp = RenderTarget->GetViewport();
	GetGraphicsCommandList()->RSSetViewports(1, &ViewPort_temp);
	D3D12_RECT ScissorRect_temp = RenderTarget->GetScissorRect();
	GetGraphicsCommandList()->RSSetScissorRects(1, &ScissorRect_temp);

	for (SIZE_T i = 0; i < 6; i++)
	{
		GetGraphicsCommandList()->ClearRenderTargetView(RenderTarget->GetRenderTargetDescriptor()[i], DirectX::Colors::Black, 0, nullptr);
		GetGraphicsCommandList()->ClearDepthStencilView(DSVDescriptor, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
		//给RenderTarget和DepthSencil设置Resource Descriptor handle
		GetGraphicsCommandList()->OMSetRenderTargets(1, &RenderTarget->GetRenderTargetDescriptor()[i], true, &DSVDescriptor);
	
		GeometryMap->Draw(DeltaTime);
		FRenderLayerManage::GetRenderLayerManage()->Draw(DeltaTime);
	}

	//Draw other content
	//MeshManage->Draw(DeltaTime);	//将图形渲染命令添加到commandList中
	//MeshManage->PostDraw(DeltaTime);

	D3D12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
		RenderTarget->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier2);

	//close commandlist and commit to commandqueue
	GetGraphicsCommandList()->Close();
	ID3D12CommandList* CommandList[] = { GetGraphicsCommandList().Get() };
	GetCommandQueue()->ExecuteCommandLists(_countof(CommandList), CommandList);

	//将画面呈现到屏幕上
	//SwapChain->Present(0, 0);
	//改变交换链索引
	//CurrentSwapBufferIndex = (CurrentSwapBufferIndex + 1) % 2;

	//cpu等待gpu执行
	//WaitGPUCommandQueueComplete();
}

void FDynamicCubeMap::BuildViewport(const XMFLOAT3& InCenterPoint)
{
	struct FTempViewportCapture
	{
		XMFLOAT3 TargetPosition[6];
		XMFLOAT3 UpDirection[6];
	};

	FTempViewportCapture Capture = {};
	//前面
	Capture.TargetPosition[0] = XMFLOAT3(InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z);
	Capture.UpDirection[0] = XMFLOAT3(0.f, 1.f, 0.f);
	//后面
	Capture.TargetPosition[1] = XMFLOAT3(InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z);
	Capture.UpDirection[1] = XMFLOAT3(0.f, 1.f, 0.f);
	//左面
	Capture.TargetPosition[2] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z);
	Capture.UpDirection[2] = XMFLOAT3(0.f, 1.f, 0.f);
	//右面
	Capture.TargetPosition[3] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z);
	Capture.UpDirection[3] = XMFLOAT3(0.f, 1.f, 0.f);
	//上面
	Capture.TargetPosition[4] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f);
	Capture.UpDirection[4] = XMFLOAT3(0.f, 0.f, -1.f);
	//下面
	Capture.TargetPosition[5] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f);
	Capture.UpDirection[5] = XMFLOAT3(0.f, 0.f, 1.f);


	for (size_t i = 0; i < 6; i++)
	{
		Viewport.emplace_back(LurenjiaEngine::CreateObject<AClientViewport>("CubeMapViewport_" + std::to_string(i)));
		AClientViewport* ClientViewport = Viewport[Viewport.size() - 1];

		ClientViewport->FaceTarget(InCenterPoint, Capture.TargetPosition[i], Capture.UpDirection[i]);
		ClientViewport->SetFrustum(0.5f * XM_PI, 1.f, 0.1f, 10000.f);
		ClientViewport->BulidViewMatrix(30.f);
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
	// 资源描述
	D3D12_RESOURCE_DESC ResourceDesc = {};
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.Alignment = 0;//资源的对齐方式
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//depthstencilbuffer的清除规则描述
	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;

	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//gpu资源都存储在堆中
	//创建一个深度模板缓冲区和一个堆，，，将此缓冲区提交到堆中
	GetD3dDevice()->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	DSVDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	DSVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;//深度模板缓冲的尺寸

	//创建深度模板资源描述符
	GetD3dDevice()->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVDescriptor);

	CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);
	GetGraphicsCommandList()->Close();
}

void FDynamicCubeMap::BuildDepthStencilDescriptor()
{
	// 通过d3d的驱动，获取DSV描述符的大小
	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	// 获取DSV描述符堆的首地址，然后向后偏移1位（因为程序中有两个DSV，一个是最终的深度模板视图，一个是CubeMap的深度模板视图）
	DSVDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(), 1, size);

}

void FDynamicCubeMap::BuildRenderTargetDescriptor()
{
	RenderTarget->BuildRenderTargetDescriptor();
}

void FDynamicCubeMap::BuildShaderSourceDescriptor()
{
	RenderTarget->BuildShaderResourceDescriptor();
}
