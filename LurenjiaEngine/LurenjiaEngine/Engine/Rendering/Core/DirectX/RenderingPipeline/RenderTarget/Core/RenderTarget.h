#pragma once

#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "../../../../../../Interface/DirectXDeviceInterface.h"

class FRenderTarget : public IDirectXDeviceInterface, std::enable_shared_from_this<FRenderTarget>
{
public:
	FRenderTarget(int inWidth, int inHeight, DXGI_FORMAT inFormat);

	virtual void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);

	// ����RTV��Դ
	virtual void BuildRenderTargetResource();

	// ����SRVHandle �� SRV
	virtual void BuildShaderResourceDescriptorHandle();
	virtual void BuildShaderResourceView();

	virtual void ResetRenderTarget(UINT InWidth, UINT InHeight);

public:
	// ���ⲿ�ṩ�Ľӿ�
	FORCEINLINE ID3D12Resource* GetRenderTarget() const { return RenderTargetResource.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }

	// ͨ���������������ã���������Ӧhandle
	FORCEINLINE CD3DX12_CPU_DESCRIPTOR_HANDLE& GetShaderResourceDescriptorCPU() { return ShaderResourceDescriptorHandle_CPU; }
	FORCEINLINE CD3DX12_GPU_DESCRIPTOR_HANDLE& GetShaderResourceDescriptorGPU() { return ShaderResourceDescriptorHandle_GPU; }

	// ��������˽�б���
	void ResetViewport(UINT InWidth, UINT InHeight);
	void ResetScissorRect(UINT InWidth, UINT InHeight);
protected:
	UINT Width;				// �����Ŀ�
	UINT Height;			// �����ĸ�
	DXGI_FORMAT Format;		// rtv�ĸ�ʽ

	D3D12_VIEWPORT Viewport;// �ӿ�
	D3D12_RECT ScissorRect;

	ComPtr<ID3D12Resource> RenderTargetResource;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ShaderResourceDescriptorHandle_CPU;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ShaderResourceDescriptorHandle_GPU;
};

#endif

