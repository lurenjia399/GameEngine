#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(AMesh* InMesh, const FMeshRenderingData& InRenderingData)
{
	GeometryMap.BuildMeshDescData(InMesh, InRenderingData);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPiepelineState.ResetGPSDesc();
	//绑定根签名
	if (!DirectXRootSignature.Build())
	{
		Engine_Log_Error("构建根签名失败");
	}
	DirectXPiepelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());
	//绑定着色器
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "VertexShaderMain", "vs_5_0");
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "PixelShaderMain", "ps_5_0");
	DirectXPiepelineState.BindShader(&VertexShader, &PixelShader);
	//绑定输入输出布局
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPiepelineState.BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
	//创建模型资源（顶点和索引）缓冲区
	GeometryMap.BuildMeshBuffer();
	//创建描述符堆（用于存放常量缓冲描述符）
	GeometryMap.BuildDescriptorHeap();
	//构建模型的常量缓冲区
	GeometryMap.BuildMeshConstantBufferView();
	//构建材质的常量缓冲区
	GeometryMap.BuildMaterialConstantBufferView();
	//构建灯光的常量缓冲区
	GeometryMap.BuildLightConstantBufferView();
	//构建视口的常量缓冲区
	GeometryMap.BuildViewportConstantBufferView();
	
	DirectXPiepelineState.Build();
}

void FRenderingPipeline::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	GeometryMap.UpdateConstantView(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPiepelineState.PreDraw(DeltaTime);
	
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	GeometryMap.PreDraw(DeltaTime);
	DirectXRootSignature.PreDraw(DeltaTime);
	GeometryMap.Draw(DeltaTime);
	DirectXPiepelineState.Draw(DeltaTime);//用做捕获keyboard 4 5
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
}
