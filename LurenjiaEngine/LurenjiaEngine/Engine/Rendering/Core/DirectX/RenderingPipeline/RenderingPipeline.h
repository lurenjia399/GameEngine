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
	FShader VertexShader;									//������ɫ��
	FShader PixelShader;									//ƬԪ��ɫ��
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		//��ɫ�����벼��

	FGeometryMap GeometryMap;								//ģ������
	FDirectXRootSignature DirectXRootSignature;				//��ǩ��
	FDirectXPiepelineState DirectXPiepelineState;			//pso
};