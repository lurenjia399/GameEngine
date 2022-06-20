#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMeshComponent(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData)
{
	GeometryMap.BuildMeshDescData(InMesh, InRenderingData);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPiepelineState.ResetGPSDesc();

	//创建贴图资源
	GeometryMap.LoadTexture();
	//绑定根签名
	if (!DirectXRootSignature.Build(GeometryMap.GetDrawTextureObjectCount()))
	{
		Engine_Log_Error("构建根签名失败");
	}
	DirectXPiepelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());
	//绑定着色器
	char buffer[10] = { 0 };
	D3D_SHADER_MACRO ShaderMacro[] =
	{
		"MapCount", _itoa(GeometryMap.GetDrawTextureObjectCount(), buffer, 10),
		nullptr, nullptr,
	};
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "VertexShaderMain", "vs_5_1", ShaderMacro);
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/main.hlsl", "PixelShaderMain", "ps_5_1", ShaderMacro);
	DirectXPiepelineState.BindShader(&VertexShader, &PixelShader);
	//绑定输入布局
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"UTANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPiepelineState.BindInputLayout(InputElementDesc.data(), (UINT)InputElementDesc.size());
	//创建模型资源（顶点和索引）缓冲区
	GeometryMap.BuildMeshBuffer();
	//创建描述符堆（用于存放常量缓冲区中的资源）
	GeometryMap.BuildDescriptorHeap();
	//构建模型的常量缓冲区
	GeometryMap.BuildMeshConstantBufferView();
	//构建材质的shader资源缓冲区,收集所有的matrial，并且设置material中的shaderindex
	GeometryMap.BuildMaterialShaderResourseView();
	//构建灯光的常量缓冲区
	GeometryMap.BuildLightConstantBufferView();
	//构建视口的常量缓冲区
	GeometryMap.BuildViewportConstantBufferView();
	//构建贴图的着色器资源
	GeometryMap.BuildTextureShaderResource();
	
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
