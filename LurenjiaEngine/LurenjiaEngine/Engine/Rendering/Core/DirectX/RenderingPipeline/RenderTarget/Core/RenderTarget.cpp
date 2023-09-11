#include "RenderTarget.h"

FRenderTarget::FRenderTarget(int inWidth, int inHeight, DXGI_FORMAT inFormat)
	: Width(inWidth)
	, Height(inHeight)
	, Format(inFormat)
{
	ResetViewport(Width, Height);
	ResetScissorRect(Width, Height);
}

void FRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	Width = InWidth;
	Height = InHeight;
	Format = InFormat;

	ResetViewport(InWidth, InHeight);
	ResetScissorRect(InWidth, InHeight);

	BuildResource();

	// Ӧ��Ҫ��ס���ڴ���view֮ǰ��Ҫ����DescriptorHandle
	// ��Ҫ�����Ǵ���������Դ���view

	BuildShaderResourceDescriptorHandle();//Ӧ������������������ɶ��û�У���Ҫ����
	BuildShaderResourceView();

	BuildDepthStencilDescriptorHandle();
	BuildDepthStencilView();
}

void FRenderTarget::BuildResource()
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FRenderTarget::BuildRenderTargetMap D3dDevice is nullptr");
		return;
	}

}

void FRenderTarget::BuildShaderResourceDescriptorHandle()
{
	// ���� ShaderResourceDescriptorHandle_CPU 
	// ���� ShaderResourceDescriptorHandle_GPU
	// ������û���ⴴ�������˷��������ڱ���ļ�������
}

void FRenderTarget::BuildShaderResourceView()
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

void FRenderTarget::BuildDepthStencilDescriptorHandle()
{
}

void FRenderTarget::BuildDepthStencilView()
{
}

void FRenderTarget::ResetRenderTarget(UINT InWidth, UINT InHeight)
{
	Width = InWidth;
	Height = InHeight;

	// ���´���RTVResource
	BuildResource();

	// ���´���SRVHandle��SRV
	BuildShaderResourceDescriptorHandle();
	BuildShaderResourceView();

	// ���´���dsvHandle��dsv
	BuildDepthStencilDescriptorHandle();
	BuildDepthStencilView();
}

void FRenderTarget::ResetViewport(UINT InWidth, UINT InHeight)
{
	Viewport = {
		0.f,			// TopLeftX
		0.f,			// TopLeftY
		(float)InWidth,	// Width
		(float)InHeight,// Height
		0.f,			// MinDepth
		1.f				// MaxDepth
	};
}

void FRenderTarget::ResetScissorRect(UINT InWidth, UINT InHeight)
{
	ScissorRect = {
		0,				// left
		0,				// top
		(LONG)InWidth,	// right
		(LONG)InHeight	// bottom
	};
}
