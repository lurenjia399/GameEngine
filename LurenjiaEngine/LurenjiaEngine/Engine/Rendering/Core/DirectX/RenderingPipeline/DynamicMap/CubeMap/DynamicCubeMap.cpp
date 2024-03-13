#include "DynamicCubeMap.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../../Core/Viewport/ViewportTransformation.h"
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
		Engine_Log_Error("RenderTarget dynamic_cast ʧ��");
		return;
	}
	for (UINT j = 0; j < GeometryMap->DynamicReflectionMeshComponents.size(); ++j)
	{
		// �ҵ������ܹ����������
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
				// �ⲿ�־��Ǹ��������λ�ã���6��rtv��ͼȫ����ȡ�ɹ�
				for (SIZE_T i = 0; i < 6; i++)
				{
					GetGraphicsCommandList()->ClearRenderTargetView(CubeMapRenderTarget->GetRenderTargetDescriptor()[i], DirectX::Colors::Black, 0, nullptr);
					GetGraphicsCommandList()->ClearDepthStencilView(CubeMapRenderTarget->GetDepthStencilDescriptor(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
					// ��仰����˼�ǣ����õ�ǰ��rendertarget�������⿴��һ����ͼ��������draw����ִ�����rendertaargetview�ͱ����һ��������ͼ
					GetGraphicsCommandList()->OMSetRenderTargets(1, &CubeMapRenderTarget->GetRenderTargetDescriptor()[i], true, &CubeMapRenderTarget->GetDepthStencilDescriptor());

					// �ⲿ�ֶ����ڸ�rtv�ϻ��ƶ���
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

		// ��ʼ���ý����������Ǹ�rtv
		StartSetMainViewportRenderTarget();

		// ���ø�ǩ�����������������ȥ
		GeometryMap->DrawViewport(DeltaTime);

		// �ⲿ�־�������ɫ���ϴ���6���������鿴rootsignature���֪��6������cubemap����
		// Ҳ���Ƕ���Ӧģ����Ⱦ��ʱ����cubemap��ͼ��������Ⱦ
		{
			// ���ø�ǩ��6��Ҳ����cubemap��srv
			Draw(DeltaTime);

			// ����Ӧģ�ͽ�����Ⱦ
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
		// ע������ط���outer���˸�nullptr
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
	// ��ΪGeometryMap���ֵ��FCubeMapRenderTarget�������ȡ������
	// ������RenderTarget->Init��ʼǰ��������ShaderResourceDescriptorHandle��cpu���ڴ���srv��gpu���ڰ󶨸�ǩ��
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
	// ע������Ϊʲô�����˳�򣬲��Ǻ���⣬cubemap��˳��ô?
	// 
	//����
	TargetPosition[0] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z);
	UpDirection[0] = XMFLOAT3(0.f, 0.f, 1.f);
	//����
	TargetPosition[1] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z);
	UpDirection[1] = XMFLOAT3(0.f, 0.f, 1.f);
	//����
	TargetPosition[2] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f);
	UpDirection[2] = XMFLOAT3(-1.f, 0.f, 0.f);
	//����
	TargetPosition[3] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f);
	UpDirection[3] = XMFLOAT3(1.f, 0.f, 0.f);
	//ǰ��
	TargetPosition[4] = XMFLOAT3(InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z);
	UpDirection[4] = XMFLOAT3(0.f, 0.f, 1.f);
	//����
	TargetPosition[5] = XMFLOAT3(InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z);
	UpDirection[5] = XMFLOAT3(0.f, 0.f, 1.f);
}
