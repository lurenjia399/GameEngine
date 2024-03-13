#include "DynamicShadowMap.h"
#include "../../RenderTarget/ShadowMapRenderTarget.h"
#include "../../Geometry/GeometryMap.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"
#include "../../RenderingLayer/RenderLayerManage.h"

FDynamicShadowMap::FDynamicShadowMap()
	: Super(512, 512, nullptr, nullptr)
{
	CreateRenderTarget<FShadowMapRenderTarget>();
}

void FDynamicShadowMap::UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	Super::UpdateViewportConstantBufferView(DeltaTime, ViewportInfo);
	//更新视口
	if (Viewport)
	{
		FViewportInfo ShadowViewportInfo;
		GetViewportMatrix(ShadowViewportInfo.ViewMatrix, ShadowViewportInfo.ProjectMatrix);
		ShadowViewportInfo.cameraPosition =
			XMFLOAT4(
				Viewport->GetPosition().x,
				Viewport->GetPosition().y,
				Viewport->GetPosition().z,
				1.f);

		GeometryMap->UpdateViewportConstantBufferView(
			DeltaTime,
			ShadowViewportInfo,
			GeometryMap->GetDynamicReflectionViewportNum() + //动态反射的摄像机
			1);//主视口
	}
}

void FDynamicShadowMap::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	Super::Init(InGeometryMap, InDirectXPiepelineState);
}

void FDynamicShadowMap::PreDraw(float DeltaTime)
{
	
}

void FDynamicShadowMap::Draw(float DeltaTime)
{
	FShadowMapRenderTarget* ShadowMapRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(RenderTarget.get());
	if (!ShadowMapRenderTarget)
	{
		Engine_Log_Error("ShadowMapRenderTarget dynamic_cast 失败");
		return;
	}

	D3D12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		ShadowMapRenderTarget->GetRenderTarget(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);

	D3D12_VIEWPORT ViewPort_temp = ShadowMapRenderTarget->GetViewport();
	GetGraphicsCommandList()->RSSetViewports(1, &ViewPort_temp);
	D3D12_RECT ScissorRect_temp = ShadowMapRenderTarget->GetScissorRect();
	GetGraphicsCommandList()->RSSetScissorRects(1, &ScissorRect_temp);

	GetGraphicsCommandList()->ClearDepthStencilView(ShadowMapRenderTarget->GetDepthStencilDescriptor(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
	// 这句话的意思是，设置当前的rendertarget（这玩意看成一张贴图），后面draw命令执行完后，rendertaargetview就变成了一张完整贴图
	GetGraphicsCommandList()->OMSetRenderTargets(0, nullptr, false, &ShadowMapRenderTarget->GetDepthStencilDescriptor());

	UINT ViewportByteSize = GeometryMap->ViewportConstantBufferView.GetBufferByteSize();
	auto ViewportAddr = GeometryMap->ViewportConstantBufferView.GetBuffer()->GetGPUVirtualAddress();
	ViewportAddr += (1 + GeometryMap->GetDynamicReflectionViewportNum()) * ViewportByteSize;
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddr);

	DrawShadowMapTexture(DeltaTime);

	// 渲染阴影层级
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUESHADOW, DeltaTime);

	D3D12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
		ShadowMapRenderTarget->GetRenderTarget(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
	GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier2);
}

void FDynamicShadowMap::BuildViewport()
{
	// 注意这个地方的outer传了个nullptr
	Viewport = LurenjiaEngine::CreateObject<AClientViewport>(nullptr, "ShadowMapViewport");
}

void FDynamicShadowMap::SetViewportPosition(const XMFLOAT3& InCenterPoint)
{
	Viewport->SetPosition(InCenterPoint);
	BuildViewMatrix(0.3f);
}

void FDynamicShadowMap::SetViewportRotation(const fvector_3d& InCenterPoint)
{
	Viewport->SetRotation(InCenterPoint);
	BuildViewMatrix(0.3f);
}

void FDynamicShadowMap::BuildViewMatrix(float DeltaTime)
{
	Viewport->BulidViewMatrix(DeltaTime);
}

void FDynamicShadowMap::BuildViewport(const XMFLOAT3& InDirection, const XMFLOAT3& InTargetPoint, float InRadius)
{
	// 摄像机位置
	XMFLOAT3 ViewportPosition = XMFLOAT3(InDirection.x * InRadius * 0.1f, InDirection.y * InRadius * 0.1f, InDirection.z * InRadius * 0.1f);
	Viewport->SetPosition(ViewportPosition);
	
	// 目标点，这里是确定摄像机的那三个轴
	Viewport->FaceTarget(ViewportPosition, InTargetPoint);

	// 构建viewMatrix
	Viewport->BulidViewMatrix(3.0f);

	// 构建orthMatrix
	Viewport->BuildOrthMatrix(InRadius, InTargetPoint);

	// 设置一下视口的大小
	Viewport->ViewPortInfo = RenderTarget->GetViewport();
	Viewport->ViewPortRect = RenderTarget->GetScissorRect();
}

void FDynamicShadowMap::GetViewportMatrix(XMFLOAT4X4& OutViewMatrix, XMFLOAT4X4& OutProjectMatrix)
{
	if (Viewport)
	{
		OutViewMatrix = Viewport->ViewMatrix;
		OutProjectMatrix = Viewport->ProjectMatrix;
	}
}

void FDynamicShadowMap::DrawShadowMapTexture(float DeltatTime)
{
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(7, RenderTarget->GetShaderResourceDescriptorGPU());
}

void FDynamicShadowMap::BuildRenderTargetDescriptor()
{
	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_CPU_DESCRIPTOR_HANDLE CPU_SRVHeapStart = GetCBV_SRV_UAVHeap()->GetRenderingHeap()->GetCPUDescriptorHandleForHeapStart();
	RenderTarget->GetShaderResourceDescriptorCPU() = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		CPU_SRVHeapStart,
		GeometryMap->GetDrawTextureObjectCount() + GeometryMap->GetDrawCubeMapCount() + 1,
		size);

	D3D12_GPU_DESCRIPTOR_HANDLE GPU_SRVHeapStart = GetCBV_SRV_UAVHeap()->GetRenderingHeap()->GetGPUDescriptorHandleForHeapStart();
	RenderTarget->GetShaderResourceDescriptorGPU() = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		GPU_SRVHeapStart,
		GeometryMap->GetDrawTextureObjectCount() + GeometryMap->GetDrawCubeMapCount() + 1,
		size);

	RenderTarget->Init(Width, Height, DXGI_FORMAT_D24_UNORM_S8_UINT);
}

void FDynamicShadowMap::BuildDepthStencilDescriptorHandle()
{
	RenderTarget->BuildDepthStencilDescriptorHandle();
}

void FDynamicShadowMap::BuildShaderResourceDescriptorHandle()
{

}

void FDynamicShadowMap::BuildRenderTargetSRV()
{
	
}
