#include "DynamicCubeMap.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../../Core/Viewport/ViewportConstantBuffer.h"
#include "../../../../../../Core/Construction/ObjectConstruction.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"



FDynamicCubeMap::FDynamicCubeMap()
	: Super(512, 512, nullptr, nullptr)
	, Viewport({})
{
	//RenderTarget = make_shared<FCubeMapRenderTarget>();
	CreateRenderTarget<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	if (Viewport.size() == 6)
	{
		for (UINT i = 0; i < GeometryMap->DynamicReflectionMeshComponents.size(); i++)
		{
			CMeshComponent* Tmp = GeometryMap->DynamicReflectionMeshComponents[i];
			XMFLOAT3 F3 = Tmp->GetPosition();
			SetViewportPosition(F3);

			for (UINT j = 0; j < 6; j++)
			{
				FViewportInfo MyViewportInfo = {};
				MyViewportInfo.cameraPosition = XMFLOAT4(Viewport[j]->GetPosition().x, Viewport[j]->GetPosition().y, Viewport[j]->GetPosition().z, 1.0f);
				MyViewportInfo.ViewMatrix = Viewport[j]->ViewMatrix;
				MyViewportInfo.ProjectMatrix = Viewport[j]->ProjectMatrix;
				GeometryMap->UpdateViewportConstantBufferView(DeltaTime, MyViewportInfo, j + i * 6 + 1);
			}
		}
	}
}

void FDynamicCubeMap::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	Super::Init(InGeometryMap, InDirectXPiepelineState);
}

void FDynamicCubeMap::PreDraw(float DeltaTime)
{
	FCubeMapRenderTarget* CubeMapRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(RenderTarget.get());
	if (! CubeMapRenderTarget)
	{
		Engine_Log_Error("RenderTarget dynamic_cast 失败");
		return;
	}
	for (UINT j = 0; j < GeometryMap->DynamicReflectionMeshComponents.size(); ++j)
	{
		// 找到所有能够反射的物体
		{
			
			D3D12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				CubeMapRenderTarget->GetRenderTarget(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);

			D3D12_VIEWPORT ViewPort_temp = CubeMapRenderTarget->GetViewport();
			GetGraphicsCommandList()->RSSetViewports(1, &ViewPort_temp);
			D3D12_RECT ScissorRect_temp = CubeMapRenderTarget->GetScissorRect();
			GetGraphicsCommandList()->RSSetScissorRects(1, &ScissorRect_temp);

			UINT ViewportByteSize = GeometryMap->ViewportConstantBufferView.GetBufferByteSize();
			{
				// 这部分就是根据摄像机位置，把6张rtv贴图全部获取成功
				for (SIZE_T i = 0; i < 6; i++)
				{
					GetGraphicsCommandList()->ClearRenderTargetView(CubeMapRenderTarget->GetRenderTargetDescriptor()[i], DirectX::Colors::Black, 0, nullptr);
					GetGraphicsCommandList()->ClearDepthStencilView(CubeMapRenderTarget->GetDepthStencilDescriptor(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
					// 这句话的意思是，设置当前的rendertarget（这玩意看成一张贴图），后面draw命令执行完后，rendertaargetview就变成了一张完整贴图
					GetGraphicsCommandList()->OMSetRenderTargets(1, &CubeMapRenderTarget->GetRenderTargetDescriptor()[i], true, &CubeMapRenderTarget->GetDepthStencilDescriptor());

					// 这部分都是在给rtv上绘制东西
					{
						auto ViewportAddr = GeometryMap->ViewportConstantBufferView.GetBuffer()->GetGPUVirtualAddress();
						ViewportAddr += (1 + i + j * 6) * ViewportByteSize;
						GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddr);



						FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND, DeltaTime);
						FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE, DeltaTime);
						FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_TRANSPARENT, DeltaTime);
					}
				}
			}

			D3D12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
				CubeMapRenderTarget->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier2);
		}

		// 开始设置交换链上面那个rtv
		StartSetMainViewportRenderTarget();

		// 设置跟签名，把主摄像机传进去
		GeometryMap->DrawViewport(DeltaTime);

		// 这部分就是往着色器上传递6参数，，查看rootsignature后可知，6参数是cubemap参数
		// 也就是对相应模型渲染的时候，用cubemap贴图来进行渲染
		{
			// 设置跟签名6，也就是cubemap的srv
			Draw(DeltaTime);

			// 对相应模型进行渲染
			FRenderLayerManage::GetRenderLayerManage()->DrawObjectByLayer(
				DeltaTime,
				(int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUEREFLECT,
				GeometryMap->DynamicReflectionMeshComponents[j]);
		}

		EndSetMainViewportRenderTarget();
	}
}

void FDynamicCubeMap::Draw(float DeltaTime)
{
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, RenderTarget->GetShaderResourceDescriptorGPU());
}

void FDynamicCubeMap::BuildViewport(const XMFLOAT3& InCenterPoint)
{
	FTempViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++)
	{
		// 注意这个地方的outer传了个nullptr
		Viewport.emplace_back(LurenjiaEngine::CreateObject<AClientViewport>(nullptr, "CubeMapViewport_" + std::to_string(i)));
		shared_ptr<AClientViewport> ClientViewport = Viewport[Viewport.size() - 1];
		
		ClientViewport->SetPosition(InCenterPoint);
		ClientViewport->FaceTarget(InCenterPoint, Capture.TargetPosition[i], Capture.UpDirection[i]);
		ClientViewport->SetFrustum(0.5f * XM_PI, 1.f, 0.1f, 10000.f);
		ClientViewport->BulidViewMatrix(30.f);

		ClientViewport->ViewPortInfo = RenderTarget->GetViewport();
		ClientViewport->ViewPortRect = RenderTarget->GetScissorRect();
	}
}

void FDynamicCubeMap::BuildDepthStencilView()
{
	RenderTarget->BuildDepthStencilView();
}

void FDynamicCubeMap::BuildDepthStencilDescriptorHandle()
{
	RenderTarget->BuildDepthStencilDescriptorHandle();
}

void FDynamicCubeMap::BuildRenderTargetDescriptor()
{
	// 因为GeometryMap这个值在FCubeMapRenderTarget这个里面取不到，
	// 所以在RenderTarget->Init开始前，创建了ShaderResourceDescriptorHandle，cpu用于创建srv，gpu用于绑定跟签名
	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	D3D12_CPU_DESCRIPTOR_HANDLE CPU_SRVHeapStart = GetCBV_SRV_UAVHeap()->GetRenderingHeap()->GetCPUDescriptorHandleForHeapStart();
	RenderTarget->GetShaderResourceDescriptorCPU() = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		CPU_SRVHeapStart,
		GeometryMap->GetDrawTextureObjectCount() + GeometryMap->GetDrawCubeMapCount(),
		size);

	D3D12_GPU_DESCRIPTOR_HANDLE GPU_SRVHeapStart = GetCBV_SRV_UAVHeap()->GetRenderingHeap()->GetGPUDescriptorHandleForHeapStart();
	RenderTarget->GetShaderResourceDescriptorGPU() = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		GPU_SRVHeapStart,
		GeometryMap->GetDrawTextureObjectCount() + GeometryMap->GetDrawCubeMapCount(),
		size);

	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);

	
}

void FDynamicCubeMap::BuildShaderSourceView()
{
	//UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//D3D12_CPU_DESCRIPTOR_HANDLE CPU_SRVHeapStart = GeometryMap->GetDescriptorHeap()->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	//RenderTarget->GetShaderResourceDescriptorCPU() = CD3DX12_CPU_DESCRIPTOR_HANDLE(
	//	CPU_SRVHeapStart,
	//	GeometryMap->GetDrawTextureObjectCount() + GeometryMap->GetDrawCubeMapCount(),
	//	size);

	//D3D12_GPU_DESCRIPTOR_HANDLE GPU_SRVHeapStart = GeometryMap->GetDescriptorHeap()->GetHeap()->GetGPUDescriptorHandleForHeapStart();
	//RenderTarget->GetShaderResourceDescriptorGPU() = CD3DX12_GPU_DESCRIPTOR_HANDLE(
	//	GPU_SRVHeapStart,
	//	GeometryMap->GetDrawTextureObjectCount() + GeometryMap->GetDrawCubeMapCount(),
	//	size);
	//RenderTarget->BuildShaderResourceDescriptorHandle();

	//RenderTarget->BuildShaderResourceView();
}

void FDynamicCubeMap::SetViewportPosition(const XMFLOAT3& InCenterPoint)
{
	FTempViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++)
	{

		Viewport[i]->SetPosition(InCenterPoint);
		Viewport[i]->FaceTarget(InCenterPoint, Capture.TargetPosition[i], Capture.UpDirection[i]);
		Viewport[i]->BulidViewMatrix(30.f);
	}
}

bool FDynamicCubeMap::IsExitDynamicReflectionMesh()
{

	return GeometryMap->DynamicReflectionMeshComponents.size() > 0;
}

FDynamicCubeMap::FTempViewportCapture::FTempViewportCapture()
{
}

FDynamicCubeMap::FTempViewportCapture::FTempViewportCapture(const XMFLOAT3& InCenterPoint)
{
	BuildViewportCapture(InCenterPoint);
	
}

void FDynamicCubeMap::FTempViewportCapture::BuildViewportCapture(const XMFLOAT3& InCenterPoint)
{
	// 注意这里为什么是这个顺序，不是很理解，cubemap的顺序么?
	// 
	//右面
	TargetPosition[0] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z);
	UpDirection[0] = XMFLOAT3(0.f, 0.f, 1.f);
	//左面
	TargetPosition[1] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z);
	UpDirection[1] = XMFLOAT3(0.f, 0.f, 1.f);
	//上面
	TargetPosition[2] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f);
	UpDirection[2] = XMFLOAT3(-1.f, 0.f, 0.f);
	//下面
	TargetPosition[3] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f);
	UpDirection[3] = XMFLOAT3(1.f, 0.f, 0.f);
	//前面
	TargetPosition[4] = XMFLOAT3(InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z);
	UpDirection[4] = XMFLOAT3(0.f, 0.f, 1.f);
	//后面
	TargetPosition[5] = XMFLOAT3(InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z);
	UpDirection[5] = XMFLOAT3(0.f, 0.f, 1.f);
}
