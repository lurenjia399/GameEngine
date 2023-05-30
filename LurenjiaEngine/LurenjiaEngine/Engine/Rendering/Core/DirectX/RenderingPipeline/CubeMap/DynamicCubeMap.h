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

	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState);
	virtual void Draw(float DeltaTime);
protected:
	virtual void BuildViewport(const XMFLOAT3& InCenterPoint);//���������
	virtual void BuildDepthStencil();

protected:
	std::shared_ptr<FCubeMapRenderTarget> RenderTarget;

	std::vector<AClientViewport*> Viewport;//���ǵ��ӿڣ�Ҳ���������

	FGeometryMap* GeometryMap;
	FDirectXPiepelineState* DirectXPiepelineState;

	ComPtr<ID3D12Resource> DepthStencilBuffer;
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDescriptor;
	UINT Width;
	UINT Height;
};