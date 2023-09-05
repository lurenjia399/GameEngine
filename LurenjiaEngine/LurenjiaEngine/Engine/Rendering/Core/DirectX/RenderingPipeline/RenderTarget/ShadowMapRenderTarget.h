#pragma once

#ifndef SHADOWMAPRENDERTARGET_H
#define SHADOWMAPRENDERTARGET_H

#include "Core/RenderTarget.h"


class FShadowMapRenderTarget : public FRenderTarget
{
public:
	FShadowMapRenderTarget();

	virtual void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat) override;

	virtual void BuildRenderTargetResource() override;

	virtual void BuildShaderResourceDescriptorHandle() override;
	virtual void BuildShaderResourceView() override;

	virtual void ResetRenderTarget(UINT InWidth, UINT InHeight) override;
};


#endif // 
