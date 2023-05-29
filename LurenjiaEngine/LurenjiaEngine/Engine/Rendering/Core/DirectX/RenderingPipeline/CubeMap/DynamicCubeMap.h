#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"

class FCubeMapRenderTarget;
class AClientViewport;
class FGeometryMap;
class FDirectXPiepelineState;

class FDynamicCubeMap : public IDirectXDeviceInterface
{
public:
	FDynamicCubeMap();

	void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState);
protected:
	virtual void BuildViewport(const XMFLOAT3& InCenterPoint);//¹¹½¨ÉãÏñ»ú
	virtual void BuildDepthStencil();

protected:
	std::unique_ptr<FCubeMapRenderTarget*> RenderTarget;
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDescriptor;

	std::vector<AClientViewport*> Viewport;

	FGeometryMap* GeometryMap;
	FDirectXPiepelineState* DirectXPiepelineState;
};