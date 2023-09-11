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

	// ���handle��ûд��Ҳ����ShaderResourceDescriptorHandle_CPUû��ֵ
	virtual void BuildShaderResourceDescriptorHandle() override;
	virtual void BuildShaderResourceView() override;

	// ���handle��ûд��Ҳ����DSVDescriptorHandleû��ֵ
	virtual void BuildDepthStencilDescriptorHandle() override;
	virtual void BuildDepthStencilView() override;

	virtual void ResetRenderTarget(UINT InWidth, UINT InHeight) override;

protected:
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDescriptorHandle;
};


#endif // 
