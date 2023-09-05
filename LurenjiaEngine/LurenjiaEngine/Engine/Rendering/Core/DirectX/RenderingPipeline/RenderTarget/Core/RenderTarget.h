#pragma once

#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "../../../../../../Interface/DirectXDeviceInterface.h"

class FRenderTarget : public IDirectXDeviceInterface, std::enable_shared_from_this<FRenderTarget>
{
public:
	FRenderTarget(int inWidth, int inHeight, DXGI_FORMAT inFormat);

	virtual void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);

	// 创建RTV资源
	virtual void BuildRenderTargetResource();

	// 创建SRVHandle 和 SRV
	virtual void BuildShaderResourceDescriptorHandle();
	virtual void BuildShaderResourceView();

	virtual void ResetRenderTarget(UINT InWidth, UINT InHeight);

public:
	// 给外部提供的接口
	FORCEINLINE ID3D12Resource* GetRenderTarget() const { return RenderTargetResource.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }

	// 通过这两个返回引用，来创建对应handle
	FORCEINLINE CD3DX12_CPU_DESCRIPTOR_HANDLE& GetShaderResourceDescriptorCPU() { return ShaderResourceDescriptorHandle_CPU; }
	FORCEINLINE CD3DX12_GPU_DESCRIPTOR_HANDLE& GetShaderResourceDescriptorGPU() { return ShaderResourceDescriptorHandle_GPU; }

	// 重新设置私有变量
	void ResetViewport(UINT InWidth, UINT InHeight);
	void ResetScissorRect(UINT InWidth, UINT InHeight);
protected:
	UINT Width;				// 画布的宽
	UINT Height;			// 画布的高
	DXGI_FORMAT Format;		// rtv的格式

	D3D12_VIEWPORT Viewport;// 视口
	D3D12_RECT ScissorRect;

	ComPtr<ID3D12Resource> RenderTargetResource;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ShaderResourceDescriptorHandle_CPU;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ShaderResourceDescriptorHandle_GPU;
};

#endif

