#pragma once

#ifndef CUBEMAPRENDERTARGET_H
#define CUBEMAPRENDERTARGET_H

#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"


// cubemap��rtv��Ҳ���ǻ�����ͨ��������Ⱦ
class FCubeMapRenderTarget
	: public IDirectXDeviceInterface
	, std::enable_shared_from_this<FCubeMapRenderTarget>
{
public:
	// ��ʼ������
	FCubeMapRenderTarget();
	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);
	void BuildRenderTargetMap();
	void BuildSRVDescriptors();
	void BuildRTVDescriptors();
public:
	// ���ⲿ�ṩ�Ľӿ�
	FORCEINLINE ID3D12Resource* GetRenderTarget() const  { return RenderTargetMap.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }
	
public:
	// ��������˽�б���
	void ResetViewport(UINT InWidth, UINT InHeight);
	void ResetScissorRect(UINT InWidth, UINT InHeight);

private:
	// ˽�����ݲ���
	UINT Width;				// �����Ŀ�
	UINT Height;			// �����ĸ�
	DXGI_FORMAT Format;		// rtv�ĸ�ʽ

	D3D12_VIEWPORT Viewport;// Describes the dimensions of a viewport
	D3D12_RECT ScissorRect; // Describes the dimensions of a viewport

	ComPtr<ID3D12Resource> RenderTargetMap;

	CD3DX12_CPU_DESCRIPTOR_HANDLE CPUShaderResource_DescriptorHandle;
	CD3DX12_CPU_DESCRIPTOR_HANDLE CPURenderTarget_DescriptorHandle[6];
};


#endif // !CubeMapRenderTarget_H
