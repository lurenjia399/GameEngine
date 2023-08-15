#pragma once

#ifndef CUBEMAPRENDERTARGET_H
#define CUBEMAPRENDERTARGET_H

#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"


// cubemap的rtv，也就是画布，通过离屏渲染
/*
* 离屏渲染就是通过cpu准备一些数据，然后传递到gpu中进行计算，然后将结果放到一个画布也就是rendertargetview当中
* 然后需要显示的时候就通过交换链来显示出来
*/
class FCubeMapRenderTarget
	: public IDirectXDeviceInterface
	, std::enable_shared_from_this<FCubeMapRenderTarget>
{
public:
	// 初始化部分
	FCubeMapRenderTarget();
	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);

	void BuildRenderTargetDescriptor();
	void BuildShaderResourceDescriptor();

	void BuildRenderTargetResource();

	void BuildShaderResourceView();
	void BuildRenderTargetView();
public:
	// 给外部提供的接口
	FORCEINLINE ID3D12Resource* GetRenderTarget() const  { return RenderTargetResource.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }
	FORCEINLINE std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE>& GetRenderTargetDescriptor() { return RenderTargetDescriptor; }
	FORCEINLINE CD3DX12_CPU_DESCRIPTOR_HANDLE& GetShaderResourceDescriptorCPU()  { return ShaderResourceDescriptorCPU; }
	FORCEINLINE CD3DX12_GPU_DESCRIPTOR_HANDLE& GetShaderResourceDescriptorGPU()  { return ShaderResourceDescriptorGPU; }

	
public:
	// 重新设置私有变量
	void ResetViewport(UINT InWidth, UINT InHeight);
	void ResetScissorRect(UINT InWidth, UINT InHeight);

public:
	void ResetRenderTarget(UINT InWidth, UINT InHeight);

private:
	// 私有数据部分
	UINT Width;				// 画布的宽
	UINT Height;			// 画布的高
	DXGI_FORMAT Format;		// rtv的格式

	D3D12_VIEWPORT Viewport;// Describes the dimensions of a viewport
	D3D12_RECT ScissorRect; // Describes the dimensions of a viewport

	ComPtr<ID3D12Resource> RenderTargetResource;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ShaderResourceDescriptorCPU;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ShaderResourceDescriptorGPU;
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> RenderTargetDescriptor;
	//CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTargetDescriptor[6];
};


#endif // 
