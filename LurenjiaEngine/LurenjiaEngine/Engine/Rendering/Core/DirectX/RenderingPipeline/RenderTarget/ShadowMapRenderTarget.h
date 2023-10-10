#pragma once

#ifndef SHADOWMAPRENDERTARGET_H
#define SHADOWMAPRENDERTARGET_H

#include "Core/RenderTarget.h"


class FShadowMapRenderTarget : public FRenderTarget
{
	typedef FRenderTarget Super;
public:
	FShadowMapRenderTarget();

	virtual void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat) override;

	virtual void BuildResource() override;


	// 这个handle还没写，也就是ShaderResourceDescriptorHandle_CPU没赋值
	virtual void BuildShaderResourceDescriptorHandle() override;
	virtual void BuildShaderResourceView() override;

	virtual void BuildDepthStencilDescriptorHandle() override;
	virtual void BuildDepthStencilView() override;

	virtual void ResetRenderTarget(UINT InWidth, UINT InHeight) override;
public:
	FORCEINLINE CD3DX12_CPU_DESCRIPTOR_HANDLE& GetDepthStencilDescriptor
	
	() { return DSVDescriptorHandle; }

protected:
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDescriptorHandle;
};


#endif // 
