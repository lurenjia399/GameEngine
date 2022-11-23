#pragma once

#ifndef CUBEMAPRENDERTARGET_H
#define CUBEMAPRENDERTARGET_H

#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"


// cubemap的rtv，也就是画布，通过离屏渲染
class FCubeMapRenderTarget
	: public IDirectXDeviceInterface
	, std::enable_shared_from_this<FCubeMapRenderTarget>
{
public:
	// 初始化部分
	FCubeMapRenderTarget();
	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);
	void BuildRenderTargetMap();
	void BuildSRVDescriptors();
	void BuildRTVDescriptors();
public:
	// 给外部提供的接口
	FORCEINLINE ID3D12Resource* GetRenderTarget() const  { return RenderTargetMap.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }
	
public:
	// 重新设置私有变量
	void ResetViewport(UINT InWidth, UINT InHeight);
	void ResetScissorRect(UINT InWidth, UINT InHeight);

private:
	// 私有数据部分
	UINT Width;				// 画布的宽
	UINT Height;			// 画布的高
	DXGI_FORMAT Format;		// rtv的格式

	D3D12_VIEWPORT Viewport;// Describes the dimensions of a viewport
	D3D12_RECT ScissorRect; // Describes the dimensions of a viewport

	ComPtr<ID3D12Resource> RenderTargetMap;

	CD3DX12_CPU_DESCRIPTOR_HANDLE CPUShaderResource_DescriptorHandle;
	CD3DX12_CPU_DESCRIPTOR_HANDLE CPURenderTarget_DescriptorHandle[6];
};


#endif // !CubeMapRenderTarget_H
