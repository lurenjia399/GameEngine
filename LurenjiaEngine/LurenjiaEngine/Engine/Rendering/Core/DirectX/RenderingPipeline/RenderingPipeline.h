#pragma once
#include "../../../../Core/Engine.h"
#include "../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "Geometry/GeometryMap.h"
#include "PiepelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"


class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(CMesh* InMesh, const FMeshRenderingData& InRenderingData);
	void BuildPipeline();
	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

protected:
	FShader VertexShader;									//顶点着色器
	FShader PixelShader;									//片元着色器
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		//着色器输入布局

	FGeometryMap GeometryMap;								//模型容器
	FDirectXRootSignature DirectXRootSignature;				//根签名
	FDirectXPiepelineState DirectXPiepelineState;			//pso
};