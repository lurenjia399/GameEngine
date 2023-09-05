#include "DynamicCubeMap.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../../Core/Construction/ObjectConstruction.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"



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
		for (UINT i = 0; i < GeometryMap->DynamicReflectionMeshComponents.size(); i++)
		{
			CMeshComponent* Tmp = GeometryMap->DynamicReflectionMeshComponents[i];
			XMFLOAT3 F3 = Tmp->GetPosition();
			SetCubeMapViewportPosition(F3);

			for (size_t j = 0; j < 6; j++)
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
	GeometryMap = InGeometryMap;
	DirectXPiepelineState = InDirectXPiepelineState;
}

void FDynamicCubeMap::PreDraw(float DeltaTime)
{

	for (UINT j = 0; j < GeometryMap->DynamicReflectionMeshComponents.size(); ++j)
	{
		// �ҵ������ܹ����������
		{
			D3D12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				RenderTarget->GetRenderTarget(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);

			D3D12_VIEWPORT ViewPort_temp = RenderTarget->GetViewport();
			GetGraphicsCommandList()->RSSetViewports(1, &ViewPort_temp);
			D3D12_RECT ScissorRect_temp = RenderTarget->GetScissorRect();
			GetGraphicsCommandList()->RSSetScissorRects(1, &ScissorRect_temp);

			UINT ViewportByteSize = GeometryMap->ViewportConstantBufferView.GetBufferByteSize();
			{
				// �ⲿ�־��Ǹ��������λ�ã���6��rtv��ͼȫ����ȡ�ɹ�
				for (SIZE_T i = 0; i < 6; i++)
				{

					GetGraphicsCommandList()->ClearRenderTargetView(RenderTarget->GetRenderTargetDescriptor()[i], DirectX::Colors::Black, 0, nullptr);
					GetGraphicsCommandList()->ClearDepthStencilView(DSVDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);
					// ��仰����˼�ǣ����õ�ǰ��rendertarget�������⿴��һ����ͼ��������draw����ִ�����rendertaargetview�ͱ����һ��������ͼ
					GetGraphicsCommandList()->OMSetRenderTargets(1, &RenderTarget->GetRenderTargetDescriptor()[i], true, &DSVDescriptorHandle);

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
				RenderTarget->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
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
			FRenderLayerManage::GetRenderLayerManage()->FindObjectDraw(
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
		Viewport.emplace_back(LurenjiaEngine::CreateObject<AClientViewport>("CubeMapViewport_" + std::to_string(i)));
		AClientViewport* ClientViewport = Viewport[Viewport.size() - 1];
		
		ClientViewport->SetPosition(InCenterPoint);
		ClientViewport->FaceTarget(InCenterPoint, Capture.TargetPosition[i], Capture.UpDirection[i]);
		ClientViewport->SetFrustum(0.5f * XM_PI, 1.f, 0.1f, 10000.f);
		ClientViewport->BulidViewMatrix(30.f);
	}
}

void FDynamicCubeMap::BuildDepthStencilView()
{
	// ��Դ����
	D3D12_RESOURCE_DESC ResourceDesc = {};
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.Alignment = 0;//��Դ�Ķ��뷽ʽ
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//depthstencilbuffer�������������
	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;

	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//gpu��Դ���洢�ڶ���
	//����һ�����ģ�建������һ���ѣ��������˻������ύ������
	GetD3dDevice()->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&ClearValue,
		IID_PPV_ARGS(DepthStencilResource.GetAddressOf()));

	//�������ģ����Դ������
	GetD3dDevice()->CreateDepthStencilView(DepthStencilResource.Get(), nullptr, DSVDescriptorHandle);
}

void FDynamicCubeMap::BuildDepthStencilDescriptorHandle()
{
	// ͨ��d3d����������ȡDSV�������Ĵ�С
	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	// ��ȡDSV�������ѵ��׵�ַ��Ȼ�����ƫ��1λ����Ϊ������������DSV��һ�������յ����ģ����ͼ��һ����CubeMap�����ģ����ͼ��
	DSVDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(), 1, size);

}

void FDynamicCubeMap::BuildRenderTargetDescriptor()
{
	// ��ΪGeometryMap���ֵ��FCubeMapRenderTarget�������ȡ������
	// ������RenderTarget->Init��ʼǰ��������ShaderResourceDescriptorHandle��cpu���ڴ���srv��gpu���ڰ󶨸�ǩ��
	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	D3D12_CPU_DESCRIPTOR_HANDLE CPU_SRVHeapStart = GeometryMap->GetDescriptorHeap()->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	RenderTarget->GetShaderResourceDescriptorCPU() = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		CPU_SRVHeapStart,
		GeometryMap->GetDrawTextureObjectCount() + GeometryMap->GetDrawCubeMapCount(),
		size);

	D3D12_GPU_DESCRIPTOR_HANDLE GPU_SRVHeapStart = GeometryMap->GetDescriptorHeap()->GetHeap()->GetGPUDescriptorHandleForHeapStart();
	RenderTarget->GetShaderResourceDescriptorGPU() = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		GPU_SRVHeapStart,
		GeometryMap->GetDrawTextureObjectCount() + GeometryMap->GetDrawCubeMapCount(),
		size);


	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);

	
}

void FDynamicCubeMap::BuildShaderSourceDescriptor()
{
	RenderTarget->BuildShaderResourceDescriptorHandle();
}

void FDynamicCubeMap::SetCubeMapViewportPosition(const XMFLOAT3& InCenterPoint)
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
