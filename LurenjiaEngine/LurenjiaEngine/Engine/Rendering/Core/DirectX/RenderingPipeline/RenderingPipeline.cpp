#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(CMesh* InMesh, const FMeshRenderingData* InRenderingData)
{
	GeometryMap.BuildMesh(InMesh, InRenderingData);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPiepelineState.ResetGPSDesc();
	DirectXPiepelineState.BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
	DirectXPiepelineState.BindRootSignature(DirectXRootSignature.RootSignature.Get());
	DirectXPiepelineState.BindShader(&VertexShader, &PixelShader);
	DirectXPiepelineState.BindRasterizerState();



	DirectXPiepelineState.Build();
}
