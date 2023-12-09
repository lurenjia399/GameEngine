#pragma once
#include "../../../../Core/Engine.h"
#include "../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../Shader/Core/Shader.h"
#include "../RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"
#include "../../RenderingTextureResourcesUpdate.h"
//#include "../../../Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"
#include "../RenderingPipeline/Geometry/GeometryMap.h"

class CDirectXRenderingEngine;

class FComputePipeline : public IDirectXDeviceInterface
{
public:
	FComputePipeline();

	void Init(FGeometryMap* InGeometryMap);

	void BuildPipeline();

	void Draw();



protected:

	 D3D12_COMPUTE_PIPELINE_STATE_DESC horzBlurPSO;
	 ComPtr<ID3D12PipelineState> pIHorzBlurPSO;
	 FShader HorzComputeShader;

	 D3D12_COMPUTE_PIPELINE_STATE_DESC VertBlurPSO;
	 ComPtr<ID3D12PipelineState> pIVertBlurPSO;
	 FShader VertComputeShader;

	 ComPtr<ID3D12RootSignature> mPostProcessRootSignature;

	 

	 shared_ptr<FRenderingUAVResourvesUpdate> pIBlurMap0;
	 shared_ptr<FRenderingUAVResourvesUpdate> pIBlurMap1;

	 FGeometryMap* GeometryMap;

	 //-----------------------------------------------------------------
	 
	 //FDirectXDescriptorHeap DescriptorHeap;// CBV_SRV_UAV这种描述符堆,,这里应该用项目中的，我就借用一下，之后必须改掉


};