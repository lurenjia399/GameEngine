#pragma once

#ifndef CUBEMAPRENDERTARGET_H
#define CUBEMAPRENDERTARGET_H

#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"


// cubemap��rtv��Ҳ���ǻ�����ͨ��������Ⱦ
/*
* ������Ⱦ����ͨ��cpu׼��һЩ���ݣ�Ȼ�󴫵ݵ�gpu�н��м��㣬Ȼ�󽫽���ŵ�һ������Ҳ����rendertargetview����
* Ȼ����Ҫ��ʾ��ʱ���ͨ������������ʾ����
*/
class FCubeMapRenderTarget
	: public IDirectXDeviceInterface
	, std::enable_shared_from_this<FCubeMapRenderTarget>
{
public:
	// ��ʼ������
	FCubeMapRenderTarget();
	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);

	void BuildRenderTargetDescriptor();
	void BuildShaderResourceDescriptor();

	void BuildRenderTargetResource();

	void BuildShaderResourceView();
	void BuildRenderTargetView();
public:
	// ���ⲿ�ṩ�Ľӿ�
	FORCEINLINE ID3D12Resource* GetRenderTarget() const  { return RenderTargetResource.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }
	FORCEINLINE std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE>& GetRenderTargetDescriptor() { return RenderTargetDescriptor; }
	FORCEINLINE CD3DX12_CPU_DESCRIPTOR_HANDLE& GetShaderResourceDescriptorCPU()  { return ShaderResourceDescriptorCPU; }
	FORCEINLINE CD3DX12_GPU_DESCRIPTOR_HANDLE& GetShaderResourceDescriptorGPU()  { return ShaderResourceDescriptorGPU; }

	
public:
	// ��������˽�б���
	void ResetViewport(UINT InWidth, UINT InHeight);
	void ResetScissorRect(UINT InWidth, UINT InHeight);

public:
	void ResetRenderTarget(UINT InWidth, UINT InHeight);

private:
	// ˽�����ݲ���
	UINT Width;				// �����Ŀ�
	UINT Height;			// �����ĸ�
	DXGI_FORMAT Format;		// rtv�ĸ�ʽ

	D3D12_VIEWPORT Viewport;// Describes the dimensions of a viewport
	D3D12_RECT ScissorRect; // Describes the dimensions of a viewport

	ComPtr<ID3D12Resource> RenderTargetResource;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ShaderResourceDescriptorCPU;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ShaderResourceDescriptorGPU;
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> RenderTargetDescriptor;
	//CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTargetDescriptor[6];
};


#endif // 
