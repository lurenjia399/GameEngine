#include "ShadowMapRenderTarget.h"

FShadowMapRenderTarget::FShadowMapRenderTarget() 
	: Super(256, 256, DXGI_FORMAT_R8G8B8A8_SNORM)
{
}

void FShadowMapRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	Super::Init(InWidth, InHeight, InFormat);
}

void FShadowMapRenderTarget::BuildResource()
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FShadowMapRenderTarget::BuildRenderTargetResource D3dDevice is nullptr");
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
	ResourceDesc.DepthOrArraySize = 1; // ���(3D��Դ)��������Դ������1D/2D��Դ��
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Format = Format;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // ��Դ�Ĳ��ָ�ʽ

	D3D12_CLEAR_VALUE ClearValue = {};
	ClearValue.DepthStencil.Depth = 1;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = Format;

	// ����һ��Ĭ�϶ѣ�Ȼ�����Դ�ŵ�Ĭ�϶�����
	D3dDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&ClearValue,
		IID_PPV_ARGS(Resource.GetAddressOf()));
}

void FShadowMapRenderTarget::BuildShaderResourceDescriptorHandle()
{
}

void FShadowMapRenderTarget::BuildShaderResourceView()
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FCubeMapRenderTarget::BuildRenderTargetMap D3dDevice is nullptr");
		return;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//������ɫ���е�float4����ɫͨ��
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.ResourceMinLODClamp = 0.f;
	SRVDesc.Texture2D.PlaneSlice = 0;

	D3dDevice->CreateShaderResourceView(
		Resource.Get(),
		&SRVDesc,
		ShaderResourceDescriptorHandle_CPU);
}

void FShadowMapRenderTarget::BuildDepthStencilDescriptorHandle()
{
	UINT size = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	DSVDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(), 1 + 1, size);
}

void FShadowMapRenderTarget::BuildDepthStencilView()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	DSVDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;//���ģ�建��ĳߴ�

	//�������ģ����Դ������
	GetD3dDevice()->CreateDepthStencilView(Resource.Get(), &DSVDesc, DSVDescriptorHandle);
}

void FShadowMapRenderTarget::ResetRenderTarget(UINT InWidth, UINT InHeight)
{
	Super::ResetRenderTarget(InWidth, InHeight);
}
