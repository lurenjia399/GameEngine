#pragma once 

#ifndef DYNAMICSHADOWMAP_H
#define DYNAMICSHADOWMAP_H

#include "../Core/DynamicMap.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"

class AClientViewport;

class FDynamicShadowMap : public FDynamicMap
{
	typedef FDynamicMap Super;

public:
	FDynamicShadowMap();

	virtual void UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo) override;
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState) override;
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;


	void BuildViewport(const XMFLOAT3& InCenterPoint);//¹¹½¨ÉãÏñ»ú
	void SetViewportPosition(const XMFLOAT3& InCenterPoint);
	void SetViewportRotation(const fvector_3d& InCenterPoint);
	void BuildViewMatrix(float DeltaTime);
	void BuildParallelLightMatrix(const XMFLOAT3& InTargetPoint, const XMFLOAT3& InDirection, float InRadius);
	void GetViewportMatrix(XMFLOAT4X4& OutViewMatrix, XMFLOAT4X4& OutProjectMatrix);

	void DrawShadowMapTexture(float DeltatTime);

public:
	virtual void BuildRenderTargetDescriptor();

	virtual void BuildDepthStencilDescriptorHandle();
	virtual void BuildShaderResourceDescriptorHandle();

protected:
	void BuildRenderTargetSRV();

protected:
	AClientViewport* Viewport;
};

#endif