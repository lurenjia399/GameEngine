#include "CubeMapRenderTarget.h"
#include "../../../../../Config/EngineRenderConfig.h"

FCubeMapRenderTarget::FCubeMapRenderTarget() : Super(256, 256, DXGI_FORMAT_R8G8B8A8_SNORM)
{
	RenderTargetDescriptor.resize(6);
}

void FCubeMapRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	Super::Init(InWidth, InHeight, InFormat);

	BuildRenderTargetDescriptorHandle();
	BuildRenderTargetView();
}

void FCubeMapRenderTarget::BuildDepthStencilDescriptorHandle()
{
	// ͨ��d3d����������ȡDSV�������Ĵ�С
	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	// ��ȡDSV�������ѵ��׵�ַ��Ȼ�����ƫ��1λ����Ϊ������������DSV��һ�������յ����ģ����ͼ��һ����CubeMap�����ģ����ͼ��
	DSVDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(), 1, size);
}

void FCubeMapRenderTarget::BuildDepthStencilView()
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

void FCubeMapRenderTarget::BuildRenderTargetDescriptorHandle()
{
	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE RTVDewcriptorStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < 6; i++)
	{
		RenderTargetDescriptor[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			RTVDewcriptorStart,
			FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,
			size);
	}
}

void FCubeMapRenderTarget::BuildShaderResourceDescriptorHandle()
{

}

void FCubeMapRenderTarget::BuildResource()
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FCubeMapRenderTarget::BuildRenderTargetResource D3dDevice is nullptr");
		return;
	}

	// �ѵ����ͣ��˳���Ĭ�϶ѣ�ֻ��gpu����
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);


	// ��Դ�����ṹ�壬������������Ĭ�϶��е���Դ
	D3D12_RESOURCE_DESC ResourceDesc = {};
	memset(&ResourceDesc, 0, sizeof(ResourceDesc));// ������
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.Alignment = 0;//��Դ�Ķ��뷽ʽ
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 6; // ���(3D��Դ)��������Դ������1D/2D��Դ��
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Format = Format;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN; // ��Դ�Ĳ��ָ�ʽ

	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&HeapProperties, 
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(Resource.GetAddressOf())));
}

void FCubeMapRenderTarget::BuildShaderResourceView()
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FCubeMapRenderTarget::BuildRenderTargetMap D3dDevice is nullptr");
		return;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = Format;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//������ɫ���е�float4����ɫͨ��
	SRVDesc.TextureCube.MostDetailedMip = 0;
	SRVDesc.TextureCube.MipLevels = 1;
	SRVDesc.TextureCube.ResourceMinLODClamp = 0.f;

	D3dDevice->CreateShaderResourceView(
		Resource.Get(),
		&SRVDesc,
		ShaderResourceDescriptorHandle_CPU);
}

void FCubeMapRenderTarget::BuildRenderTargetView()
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FCubeMapRenderTarget::BuildRenderTargetMap D3dDevice is nullptr");
		return;
	}

	for (UINT i = 0; i < 6; ++i)
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
		RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
		RTVDesc.Format = Format;
		RTVDesc.Texture2DArray.ArraySize = 1;
		RTVDesc.Texture2DArray.FirstArraySlice = i;
		RTVDesc.Texture2DArray.MipSlice = 0;
		RTVDesc.Texture2DArray.PlaneSlice = 0;
		//MipSlice + ArraySize * MipLevels

		D3dDevice->CreateRenderTargetView(Resource.Get(), &RTVDesc, RenderTargetDescriptor[i]);
	}
}


void FCubeMapRenderTarget::ResetRenderTarget(UINT InWidth, UINT InHeight)
{
	if (InWidth != Width || InHeight != Height)
	{
		Super::ResetRenderTarget(InWidth, InHeight);

		BuildRenderTargetView();
	}
	
}