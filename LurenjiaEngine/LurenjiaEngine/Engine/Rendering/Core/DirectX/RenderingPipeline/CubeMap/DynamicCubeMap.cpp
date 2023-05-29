#include "DynamicCubeMap.h"
#include "../../../../../Core/Viewport/ClientViewport.h"
#include "../../../../../Core/Construction/ObjectConstruction.h"

FDynamicCubeMap::FDynamicCubeMap()
	: GeometryMap(nullptr)
	, DirectXPiepelineState(nullptr)
{

}

void FDynamicCubeMap::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	GeometryMap = InGeometryMap;
	DirectXPiepelineState = InDirectXPiepelineState;
}

void FDynamicCubeMap::BuildViewport(const XMFLOAT3& InCenterPoint)
{
	struct FTempViewportCapture
	{
		XMFLOAT3 TargetPosition[6];
		XMFLOAT3 UpDirection[6];
	};

	FTempViewportCapture Capture = {};
	//前面
	Capture.TargetPosition[0] = XMFLOAT3(InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z);
	Capture.UpDirection[0] = XMFLOAT3(0.f, 1.f, 0.f);
	//后面
	Capture.TargetPosition[1] = XMFLOAT3(InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z);
	Capture.UpDirection[1] = XMFLOAT3(0.f, 1.f, 0.f);
	//左面
	Capture.TargetPosition[2] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z);
	Capture.UpDirection[2] = XMFLOAT3(0.f, 1.f, 0.f);
	//右面
	Capture.TargetPosition[3] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z);
	Capture.UpDirection[3] = XMFLOAT3(0.f, 1.f, 0.f);
	//上面
	Capture.TargetPosition[4] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f);
	Capture.UpDirection[4] = XMFLOAT3(0.f, 0.f, -1.f);
	//下面
	Capture.TargetPosition[5] = XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f);
	Capture.UpDirection[5] = XMFLOAT3(0.f, 0.f, 1.f);


	for (size_t i = 0; i < 6; i++)
	{
		Viewport.emplace_back(LurenjiaEngine::CreateObject<AClientViewport>("CubeMapViewport_" + std::to_string(i)));
		AClientViewport* ClientViewport = Viewport[Viewport.size() - 1];

		ClientViewport->FaceTarget(InCenterPoint, Capture.TargetPosition[i], Capture.UpDirection[i]);
		ClientViewport->SetFrustum(0.5f * XM_PI, 1.f, 0.1f, 10000.f);
		ClientViewport->BulidViewMatrix(30.f);
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
}
