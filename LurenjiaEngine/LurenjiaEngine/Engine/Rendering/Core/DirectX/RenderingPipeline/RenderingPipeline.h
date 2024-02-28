#pragma once
#include "../../../../Core/Engine.h"
#include "../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "Geometry/GeometryMap.h"
#include "PiepelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"
#include "RenderingLayer/RenderLayerManage.h"
#include "DynamicMap/CubeMap/DynamicCubeMap.h"
#include "../ComputePipeline/ComputePipeline.h"
#include "../EditorUIPipeline/IMGUIPipeline.h"



class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	bool FindMeshRenderingDataByHash(const size_t& InHashKey, std::weak_ptr<FGeometryDescData>& OutGeometryDescData, int InRenderingLayer = -1);
	void DuplicateMeshRenderingData(CMeshComponent* InMesh, std::weak_ptr<FGeometryDescData>& InGeometryDescData);
	void BuildMeshComponent(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey);
	void BuildPipeline();
	void UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo);
	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

protected:

	FGeometryMap GeometryMap;								//ģ������
	FDirectXRootSignature DirectXRootSignature;				//��ǩ��
	FDirectXPiepelineState DirectXPiepelineState;			//pso

	FDynamicCubeMap DynamicCubeMap;							//��̬����


	// �����ü�����ɫ��
	FComputePipeline ComputePipeline; //������ɫ������

	// Editor UI 
	FIMGUIPipeline EditorUIPipeline; //�༭��ui����

};