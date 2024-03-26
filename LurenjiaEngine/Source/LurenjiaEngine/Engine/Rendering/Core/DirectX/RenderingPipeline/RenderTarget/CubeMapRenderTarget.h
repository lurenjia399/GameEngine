#pragma once

#ifndef CUBEMAPRENDERTARGET_H
#define CUBEMAPRENDERTARGET_H

#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "Core/RenderTarget.h"


// cubemap的rtv，也就是画布，通过离屏渲染
/*
* 离屏渲染就是通过cpu准备一些数据，然后传递到gpu中进行计算，然后将结果放到一个画布也就是rendertargetview当中
* 然后需要显示的时候就通过交换链来显示出来
*/

class FCubeMapRenderTarget : public FRenderTarget
{
	typedef FRenderTarget Super;
public:
	// 初始化部分
	FCubeMapRenderTarget();

	virtual void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat) override;

	virtual void BuildResource() override;

	virtual void BuildShaderResourceDescriptorHandle() override;
	virtual void BuildShaderResourceView() override;

	virtual void BuildDepthStencilDescriptorHandle() override;
	virtual void BuildDepthStencilView() override;

	void BuildRenderTargetDescriptorHandle();
	void BuildRenderTargetView();

	virtual void ResetRenderTarget(UINT InWidth, UINT InHeight) override;

public:
	// 给外部提供的接口
	FORCEINLINE std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE>& GetRenderTargetDescriptor() { return RenderTargetDescriptor; }
	FORCEINLINE CD3DX12_CPU_DESCRIPTOR_HANDLE& GetDepthStencilDescriptor() { return DSVDescriptorHandle; }
	
private:
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> RenderTargetDescriptor;

	ComPtr<ID3D12Resource> DepthStencilResource;
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDescriptorHandle;
};


#endif // 
