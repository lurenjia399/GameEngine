#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../RenderingLayer/RenderLayerManage.h"

class FCubeMapRenderTarget;
class AClientViewport;
class FGeometryMap;
class FDirectXPiepelineState;

class FDynamicCubeMap : public IDirectXDeviceInterface
{
public:
	FDynamicCubeMap();

	virtual void UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo);
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState);
	virtual void PreDraw(float DeltaTime);

	virtual void BuildViewport(const XMFLOAT3& InCenterPoint);//构建摄像机
	virtual void BuildDepthStencil();

	virtual void BuildDepthStencilDescriptor();
	virtual void BuildRenderTargetDescriptor();
	virtual void BuildShaderSourceDescriptor();

protected:
	std::shared_ptr<FCubeMapRenderTarget> RenderTarget;

	std::vector<AClientViewport*> Viewport;//我们的视口，也就是摄像机

	FGeometryMap* GeometryMap;
	FDirectXPiepelineState* DirectXPiepelineState;

	ComPtr<ID3D12Resource> DepthStencilBuffer;
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDescriptor;
	UINT Width;
	UINT Height;
};