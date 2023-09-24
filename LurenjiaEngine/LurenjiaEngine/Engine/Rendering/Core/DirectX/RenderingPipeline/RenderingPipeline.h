#pragma once
#include "../../../../Core/Engine.h"
#include "../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "Geometry/GeometryMap.h"
#include "PiepelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"
#include "RenderingLayer/RenderLayerManage.h"
#include "DynamicMap/CubeMap/DynamicCubeMap.h"



class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	bool FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutGeometryDescData, int InRenderingLayer = -1);
	void DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InGeometryDescData);
	void BuildMeshComponent(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey);
	void BuildPipeline();
	void UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo);
	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

protected:

	FGeometryMap GeometryMap;								//模型容器
	FDirectXRootSignature DirectXRootSignature;				//根签名
	FDirectXPiepelineState DirectXPiepelineState;			//pso

	FDynamicCubeMap DynamicCubeMap;							//动态反射

						//阴影
};