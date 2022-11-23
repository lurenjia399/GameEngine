#include "CubeMapRenderTarget.h"

FCubeMapRenderTarget::FCubeMapRenderTarget()
	: Width(256)
	, Height(256)
	, Format(DXGI_FORMAT_R8G8B8A8_SNORM)
{
	ResetViewport(Width, Height);
	ResetScissorRect(Width, Height);
}

void FCubeMapRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	Width = InWidth;
	Height = InHeight;
	Format = InFormat;
	
	ResetViewport(InWidth, InHeight);
	ResetScissorRect(InWidth, InHeight);
}

void FCubeMapRenderTarget::BuildRenderTargetMap()
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FCubeMapRenderTarget::BuildRenderTargetMap D3dDevice is nullptr");
		return;
	}

	// 堆得类型，此出是默认堆，只供gpu访问
	D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	// 资源描述结构体，用来描述放在默认堆中的资源
	D3D12_RESOURCE_DESC ResourceDesc = {};
	memset(&ResourceDesc, 0, sizeof(ResourceDesc));// 清数据
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.Alignment = 0;//资源的对齐方式
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 6; // 深度(3D资源)或者是资源数量（1D/2D资源）
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Format = Format;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN; // 资源的布局格式

	ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
		&HeapProperties, 
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(RenderTargetMap.GetAddressOf())));
}

void FCubeMapRenderTarget::BuildSRVDescriptors()
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
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//设置着色器中的float4的颜色通道
	SRVDesc.TextureCube.MostDetailedMip = 0;
	SRVDesc.TextureCube.MipLevels = 1;
	SRVDesc.TextureCube.ResourceMinLODClamp = 0.f;

	D3dDevice->CreateShaderResourceView(
		RenderTargetMap.Get(),
		&SRVDesc,
		CPUShaderResource_DescriptorHandle);
}

void FCubeMapRenderTarget::BuildRTVDescriptors()
{
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FCubeMapRenderTarget::BuildRenderTargetMap D3dDevice is nullptr");
		return;
	}

	for (size_t i = 0; i < 6; ++i)
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
		RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
		RTVDesc.Format = Format;
		RTVDesc.Texture2DArray.ArraySize = 1;
		RTVDesc.Texture2DArray.FirstArraySlice = i;
		RTVDesc.Texture2DArray.MipSlice = 0;
		RTVDesc.Texture2DArray.PlaneSlice = 0;

		D3dDevice->CreateRenderTargetView(RenderTargetMap.Get(), &RTVDesc, CPURenderTarget_DescriptorHandle[i]);
	}
}

void FCubeMapRenderTarget::ResetViewport(UINT InWidth, UINT InHeight)
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

void FCubeMapRenderTarget::ResetScissorRect(UINT InWidth, UINT InHeight)
{
	ScissorRect = {
		0,				// left
		0,				// top
		(LONG)InWidth,	// right
		(LONG)InHeight	// bottom
	};
}
