#pragma once
#include "../../../../../../Core/Engine.h"
#include "../../RenderingLayer/RenderLayerManage.h"
#include "../Core/DynamicMap.h"

class FCubeMapRenderTarget;
class AClientViewport;
struct FGeometryMap;
struct FDirectXPiepelineState;

class FDynamicCubeMap : public FDynamicMap
{
	typedef FDynamicMap Super;

	struct FTempViewportCapture
	{
		FTempViewportCapture();
		FTempViewportCapture(const XMFLOAT3& InCenterPoint = XMFLOAT3(0.f, 0.f, 0.f));
		XMFLOAT3 TargetPosition[6];
		XMFLOAT3 UpDirection[6];

		void BuildViewportCapture(const XMFLOAT3& InCenterPoint);
	};
public:
	FDynamicCubeMap();

	virtual void UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo) override;
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState) override;
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;

	virtual void BuildViewport(const XMFLOAT3& InCenterPoint);//���������
	virtual void BuildDepthStencilView();

	virtual void BuildDepthStencilDescriptorHandle();
	virtual void BuildRenderTargetDescriptor();
	virtual void BuildShaderSourceDescriptor();

public:
	void SetCubeMapViewportPosition(const XMFLOAT3& InCenterPoint);
	bool IsExitDynamicReflectionMesh();

protected:

	std::vector<AClientViewport*> Viewport;//���ǵ��ӿڣ�Ҳ����6�������

	ComPtr<ID3D12Resource> DepthStencilResource;
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDescriptorHandle;
};